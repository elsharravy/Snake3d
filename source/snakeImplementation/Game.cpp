#include "Game.h"

#include "../engine/Random/Random.h"
#include "../engine/engine.h"

#include "../engine/resources/ResourceManager.h"

#include "GameManager.h"
#include "GameMenu.h"

#include <string>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#define SNAKE_INITIAL_VELOCITY 5
#define LIGHT_INITIAL_POS glm::vec3(5, 5, 5)

#define SKY_BOX_PATHS "resources/textures/skybox/right.png", "resources/textures/skybox/left.png","resources/textures/skybox/top.png", \
"resources/textures/skybox/bottom.png", "resources/textures/skybox/front.png", "resources/textures/skybox/back.png"


Game::Game(Engine* engine,GameManager* gameManager) : engine(engine), gameManager(gameManager),  snake(&board), pause(false), border(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f)),
border2(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f)), border3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f)),
lightSource(0.2,0.2,0.2), skyBoxCube(1, 1, 1)
{
	font = ResourceManager::getFont( Fonts_Id::MAIN_FONT );

	initializeShaders();

	initializeCubeBorders();

	initializeSkyBox();

	initializePostProcessing();
		
	lightSource.setPosition(LIGHT_INITIAL_POS);
	lightSource.setColor(glm::vec3(1.0f, 1.0f, 0.0f));

	generateRandomFood();
	initializeKeySettings();
	initializeSnake();
	initializeInterface();

	initializeSnakeSegment();
	initializeSnakeFood();

	updateViewMatrixInShaders();
}

Game::~Game()
{
	delete snakeSegment;
	delete snakeFood;
}

void Game::initializeKeySettings()
{
	keyForMovingMinusXAxis = GLFW_KEY_A;
	keyForMovingPlusXAxis = GLFW_KEY_D;
	keyForMovingMinusZAxis = GLFW_KEY_W;
	keyForMovingPlusZAxis = GLFW_KEY_S;
}

void Game::initializeSnakeFood()
{
	snakeFood = new Cube(1, 1, 1);
	snakeFood->setColor(glm::vec3(0.45f, 0.02f, 0.06f));
}

void Game::initializeSnakeSegment()
{
	snakeSegment = new Cube(1, 1, 1);
	snakeSegment->setColor(glm::vec3(0.02f, 0.29f, 0.003f));
}

void Game::initializeInterface()
{
	updateScoreText(snake.getsize());
}

void Game::initializeCubeBorders()
{
	border.setColor(glm::vec3(1.0, 1.0, 1.0));
	border2.setColor(glm::vec3(1.0, 1.0, 1.0));
	border3.setColor(glm::vec3(1.0, 1.0, 1.0));
}

void Game::initializePostProcessing()
{
	postProcessFramebuffer.generate(1920, 1080);

	setPostProcessEffect(POST_PROCESSING_EFFECT::NONE);
}

void Game::initializeSkyBox()
{
	std::vector<std::string> paths = { SKY_BOX_PATHS };

	skyBox.load(paths);

	skyBoxCube.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Game::initializeSnake()
{
	snake.setSnakeVelocity(SNAKE_INITIAL_VELOCITY);
}

void Game::initializeShaders()
{
	textShader = ResourceManager::getShader(Shaders_Id::TEXT_SHADER);

	engine->compileAndLinkShader(&colorShader, "resources/shaders/colorShader.vs", "resources/shaders/colorShader.fs");
	engine->compileAndLinkShader(&lightSourceShader, "resources/shaders/lightSource.vs", "resources/shaders/lightSource.fs");
	engine->compileAndLinkShader(&cubemapShader, "resources/shaders/cubeMap.vs", "resources/shaders/cubeMap.fs");
	engine->compileAndLinkShader(&postProcessShader, "resources/shaders/postProcess.vs", "resources/shaders/postProcess.fs");

	colorShader.use();
	colorShader.setVec3("lightColor", glm::vec3( 1.0f,1.0f,1.0f ));
	colorShader.setFloat( "ambientStrength" , 0.5 );
	colorShader.setVec3("lightPos", LIGHT_INITIAL_POS);

	cubemapShader.use();
	cubemapShader.setInt("cubemap", 0);

	glm::vec2 screenSize = engine->getscreenSize();
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screenSize.x) / screenSize.y, 0.1f, 100.0f);
	updateProjectionMatrixInShaders(projection);
}

void Game::render()
{
		// switch to off screen framebuffer
		postProcessFramebuffer.activateFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


		renderSkyBox();

		snakeFood->draw(colorShader);
		snake.draw(*snakeSegment, colorShader);
		drawBorders(colorShader);


		font->RenderText(*textShader, scoreText , 10, 10, 1.0f, glm::vec3(0.0,1.0, 0.0) );


		// draw quad to make postprocessing
		glDisable(GL_DEPTH_TEST);
		//binding default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		postProcessShader.use();
		postProcessFramebuffer.activateTexture();
		postProcessScreen.draw(postProcessShader);

		glEnable(GL_DEPTH_TEST);

	
}

void Game::update(float deltaTime)
{
	if (!pause)
	{
		Field currentField = snake.move(deltaTime);

		if (currentField == Field::SNAKE)	// we hit snake body, we lose
		{
			gameOver();
		}
		else if (currentField == Field::FOOD)	// we eat food, we need another in random centerPosition in the world
		{
			generateRandomFood();
			updateScoreText(snake.getsize());
		}
		else if (currentField == Field::WALL)	// we hit the wall
		{
			gameOver();
		}
	}

	snakeFood->setPosition(foodPos);

	updateViewMatrixInShaders();
}

void Game::updateScoreText(int score)
{
	scoreText = "Score: " + std::to_string(score);
}

void Game::gameOver()
{
//	engine->requestEngineClose();
	gameManager->getgameMenu()->gethighscores().insertNewScore( snake.getsize() );
	gameManager->getgameMenu()->gethighscores().saveToFile();
	resetGame();
	gameManager->setstate(GameState::MENU);
}

void Game::renderSkyBox()
{
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	skyBox.activate();
	skyBoxCube.draw(cubemapShader);
	glDepthMask(GL_TRUE);
}

void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void Game::mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset)
{
	if (glm::abs(xoffset) > glm::abs(yoffset))	// did we moved mouse more in x axis that y axis
	{
		cam.addAngle(-xoffset);

		setMovingKeys(cam.getAngle());
	}
	else
	{
		cam.addAngleY(yoffset);
	}
}

void Game::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
	{
		pause = !pause;
	}
	// moving snake
	else if (key == keyForMovingMinusZAxis)
	{
		snake.requestDirectionChange(glm::ivec3(0, 0, -1), glm::ivec3(0, 0, 1));
	}
	else if (key == keyForMovingPlusZAxis)
	{
		snake.requestDirectionChange(glm::ivec3(0, 0, 1), glm::ivec3(0, 0, 0));
	}
	else if (key == keyForMovingMinusXAxis)
	{
		snake.requestDirectionChange(glm::ivec3(-1, 0, 0), glm::ivec3(1, 0, 0));
	}
	else if (key == keyForMovingPlusXAxis)
	{
		snake.requestDirectionChange(glm::ivec3(1, 0, 0), glm::ivec3(0, 0, 0));
	}
	else if (key == GLFW_KEY_Q)
	{
		snake.requestDirectionChange(glm::ivec3(0, 1, 0), glm::ivec3(0, 0, 0));
	}
	else if (key == GLFW_KEY_E)
	{
		snake.requestDirectionChange(glm::ivec3(0, -1, 0), glm::ivec3(0, 1, 0));
	}
	else if (key == GLFW_KEY_ESCAPE)
	{
//		engine->requestEngineClose();
		gameManager->setstate(GameState::MENU);
	}
	else if (key == GLFW_KEY_KP_0 && action == GLFW_RELEASE)
	{
		setPostProcessEffect(POST_PROCESSING_EFFECT::NONE);
	}
	else if (key == GLFW_KEY_KP_1 && action == GLFW_RELEASE)
	{
		setPostProcessEffect(POST_PROCESSING_EFFECT::COLOR_INVERSION);
	}
	else if (key == GLFW_KEY_KP_2 && action == GLFW_RELEASE)
	{
		setPostProcessEffect(POST_PROCESSING_EFFECT::GRAYSCALE);
	}
	else if (key == GLFW_KEY_KP_3 && action == GLFW_RELEASE)
	{
		setPostProcessEffect(POST_PROCESSING_EFFECT::KERNEL);
	}
	else if (key == GLFW_KEY_M && action == GLFW_RELEASE)
	{
		gameManager->setstate( GameState::MENU );
	}

}

void Game::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.addRadius(-yoffset);

}

void Game::setMovingKeys(float angle)
{
	//	 _______	zones inside cube
	//  |\ 2  / |
	//	|  \ / 3|	
	//	|1 / \  |
	//	| / 0 \ |
	//	 -------
	angle += 45;

	int zone = ((static_cast<int>(angle / 90)) % 4);

	if (angle < 0) { zone -= 1; }

	if ((zone == 0) || (zone == -4))
	{
		keyForMovingMinusXAxis = GLFW_KEY_W;
		keyForMovingPlusXAxis = GLFW_KEY_S;
		keyForMovingMinusZAxis = GLFW_KEY_D;
		keyForMovingPlusZAxis = GLFW_KEY_A;
	}
	else if ((zone == 1) || (zone == -3))
	{
		keyForMovingMinusXAxis = GLFW_KEY_A;
		keyForMovingPlusXAxis = GLFW_KEY_D;
		keyForMovingMinusZAxis = GLFW_KEY_W;
		keyForMovingPlusZAxis = GLFW_KEY_S;
	}
	else if ((zone == 2) || (zone == -2))
	{
		keyForMovingMinusXAxis = GLFW_KEY_S;
		keyForMovingPlusXAxis = GLFW_KEY_W;
		keyForMovingMinusZAxis = GLFW_KEY_A;
		keyForMovingPlusZAxis = GLFW_KEY_D;
	}
	else if ((zone == 3) || (zone == -1))
	{
		keyForMovingMinusXAxis = GLFW_KEY_D;
		keyForMovingPlusXAxis = GLFW_KEY_A;
		keyForMovingMinusZAxis = GLFW_KEY_S;
		keyForMovingPlusZAxis = GLFW_KEY_W;
	}

}

void Game::resetGame()
{
	board = Board();
	snake = Snake(&board);
	delete snakeFood;

	initializeSnakeFood();
	generateRandomFood();
	initializeSnake();
	updateScoreText(snake.getsize());

	cam = Camera();
	setMovingKeys(cam.getAngle());
	updateViewMatrixInShaders();
}

void Game::generateRandomFood()
{
	do
	{
		foodPos = glm::ivec3(Random::randInt(0, BOARD_SIDE_SIZEX), Random::randInt(0, BOARD_SIDE_SIZEY), Random::randInt(0, BOARD_SIDE_SIZEZ));
	} while (board.getFieldState(foodPos) == Field::SNAKE);

	board.setFieldState(foodPos, Field::FOOD);
}

void Game::drawBorders( ShaderProgram& shader )
{
	// drawing border

	border.setPosition(glm::vec3(0, 0, 0));
	border2.setPosition(glm::vec3(0, 0, 0));
	border3.setPosition(glm::vec3(0, 0, 0));

	border.draw(shader);
	border2.draw(shader);
	border3.draw(shader);

	border.setPosition(glm::vec3(0, 0, 10));
	border2.setPosition(glm::vec3(10, 0, 0));
	border3.setPosition(glm::vec3(10, 0, 0));

	border.draw(shader);
	border2.draw(shader);
	border3.draw(shader);

	border.setPosition(glm::vec3(0, 10, 10));
	border2.setPosition(glm::vec3(10, 0, 10));
	border3.setPosition(glm::vec3(10, 10, 0));

	border.draw(shader);
	border2.draw(shader);
	border3.draw(shader);

	border.setPosition(glm::vec3(0, 10, 0));
	border2.setPosition(glm::vec3(0, 0, 10));
	border3.setPosition(glm::vec3(0, 10, 0));

	border.draw(shader);
	border2.draw(shader);
	border3.draw(shader);
}

void Game::updateViewMatrixInShaders()
{
	colorShader.use();
	colorShader.setMatrix4("view", cam.getView());

	lightSourceShader.use();
	lightSourceShader.setMatrix4("view", cam.getView());

	cubemapShader.use();
	glm::mat4 viewCubeMap = glm::mat4(glm::mat3(cam.getView()));
	cubemapShader.setMatrix4("view", viewCubeMap);
}

void Game::updateProjectionMatrixInShaders(glm::mat4 projection)
{
	colorShader.use();
	colorShader.setMatrix4("projection", projection);

	lightSourceShader.use();
	lightSourceShader.setMatrix4("projection", projection);

	cubemapShader.use();
	cubemapShader.setMatrix4("projection", projection);
}


void Game::setPostProcessEffect(POST_PROCESSING_EFFECT effect)
{
	activePostProcessEffect = effect;
	postProcessShader.use();
	postProcessShader.setInt("postProcessEffect", effect);
}