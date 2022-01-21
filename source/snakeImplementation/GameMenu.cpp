#include "GameMenu.h"

#include "Game.h"
#include "GameManager.h"


#include "../engine/engine.h"

GameMenu::GameMenu(GameManager* gameManager,Engine* engine) : gameManager(gameManager), engine(engine), font("resources/fonts/arial.ttf", 48)
{
	engine->compileAndLinkShader(&textShader, "resources/shaders/text/textShader.vs", "resources/shaders/text/textShader.fs");
	projection = glm::ortho(0.0f, 1900.0f, 0.0f, 1080.0f);
	font.loadCharacters();
	
	optionFocused = 0;

	textShader.use();
	textShader.setMatrix4("projection", projection);

	options.push_back(MenuOption( ));
	options.push_back(MenuOption( ));
	options.push_back(MenuOption( ));
	options.push_back(MenuOption( ));

	options.at(0).setLabel("Play", font);
	options.at(1).setLabel("Options", font);
	options.at(2).setLabel("HighScores", font);
	options.at(3).setLabel("Exit", font);

	options.at(0).setcenterPosition( glm::vec2( 960 , 800 ) );
	options.at(1).setcenterPosition(glm::vec2(960 , 600));
	options.at(2).setcenterPosition(glm::vec2(960 , 400));
	options.at(3).setcenterPosition(glm::vec2(960, 200));
}

void GameMenu::render()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < options.size(); i++)
	{
		options.at(i).draw(font, textShader);
	}

}
void GameMenu::update(float deltaTime)
{
	options.at(optionFocused).updateScale(deltaTime);
}

void GameMenu::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void GameMenu::mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset)
{

}
void GameMenu::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_M && action == GLFW_RELEASE)
	{
	gameManager->setstate(GameState::GAME);
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{

		switch (optionFocused)
		{	
		case 0:
		{
			gameManager->setstate(GameState::GAME);
			break;
		}
		case 1:
		{
//			gameManager->setstate(GameState::OPTIONS);
			break;
		}
		case 2:
		{
			
			break;
		}
		case 3:
		{
			engine->requestEngineClose();
			break;
		}
		default:
			break;
		}
	//	gameManager->setstate(GameState::GAME);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		options.at(optionFocused).resetScale();

		optionFocused += 1;

		if (optionFocused >= options.size())
		{
			optionFocused = 0;
		}
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		options.at(optionFocused).resetScale();

		optionFocused -= 1;

		if (optionFocused < 0)
		{
			optionFocused = (options.size() - 1);
		}
	}
}
void GameMenu::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{

}
void GameMenu::setPostProcessEffect(POST_PROCESSING_EFFECT effect)
{

}