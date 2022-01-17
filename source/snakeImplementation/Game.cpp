#include "Game.h"

#include "../engine/Random/Random.h"
#include "../engine/engine.h"

#define SNAKE_INITIAL_VELOCITY 5
#define LIGHT_INITIAL_POS glm::vec3(5, 5, 5)

#define SKY_BOX_PATHS "resources/textures/skybox/right.png", "resources/textures/skybox/left.png","resources/textures/skybox/top.png", \
"resources/textures/skybox/bottom.png", "resources/textures/skybox/front.png", "resources/textures/skybox/back.png"


Game::Game(Engine* engine) : engine(engine), snake(&board), pause(true), border(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f)),
border2(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f)), border3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f)),
lightSource(0.2,0.2,0.2), skyBoxCube(1, 1, 1)
{
	initializeShaders();
	initializeSkyBox();

	lightSource.setPosition(LIGHT_INITIAL_POS);
	lightSource.setColor(glm::vec3(1.0f, 1.0f, 0.0f));

	generateRandomFood();
	initializeKeySettings();
	initializeSnake();

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

void Game::initializeSkyBox()
{
	std::vector<std::string> paths = { SKY_BOX_PATHS };

	skyBox.load(paths);
}

void Game::initializeSnake()
{
	snake.setSnakeVelocity(SNAKE_INITIAL_VELOCITY);
}

void Game::initializeShaders()
{
	engine->compileAndLinkShader(&colorShader, "resources/shaders/colorShader.vs", "resources/shaders/colorShader.fs");
	engine->compileAndLinkShader(&lightSourceShader, "resources/shaders/lightSource.vs", "resources/shaders/lightSource.fs");
	engine->compileAndLinkShader(&cubemapShader, "resources/shaders/cubeMap.vs", "resources/shaders/cubeMap.fs");

	colorShader.use();
	colorShader.setVec3("lightColor", glm::vec3( 1.0f,1.0f,1.0f ));
	colorShader.setFloat( "ambientStrength" , 0.5 );
	colorShader.setVec3("lightPos", LIGHT_INITIAL_POS);

	cubemapShader.use();
	cubemapShader.setInt("cubemap", 0);
}

void Game::render()
{
//	lightSource.draw(lightSourceShader);

	renderSkyBox();

	snakeFood->draw(colorShader);
	snake.draw(*snakeSegment, colorShader);
	drawBorders(colorShader);
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
		else if (currentField == Field::FOOD)	// we eat food, we need another in random position in the world
		{
			generateRandomFood();
		}
		else if (currentField == Field::WALL)	// we hit the wall
		{
			gameOver();
		}
	}

	snakeFood->setPosition(foodPos);

	updateViewMatrixInShaders();
}

void Game::gameOver()
{
	engine->requestEngineClose();
}

void Game::renderSkyBox()
{
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	skyBox.activate();
	skyBoxCube.draw(cubemapShader);
	glDepthMask(GL_TRUE);
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
		engine->requestEngineClose();
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