#include "GameMenu.h"

#include "Game.h"
#include "GameManager.h"

#include "../engine/resources/ResourceManager.h"


#include "../engine/engine.h"

#define HIGHSCORES_PATH "resources/data/best.hs"

GameMenu::GameMenu(GameManager* gameManager,Engine* engine) : gameManager(gameManager), engine(engine), highscores(HIGHSCORES_PATH)
{
	menuOptionSelected = GameState::MENU;
//	engine->compileAndLinkShader(&textShader, "resources/shaders/text/textShader.vs", "resources/shaders/text/textShader.fs");
	textShader = ResourceManager::getShader(Shaders_Id::TEXT_SHADER);

	projection = glm::ortho(0.0f, 1900.0f, 0.0f, 1080.0f);
	
	font = ResourceManager::getFont(Fonts_Id::MAIN_FONT);

	initializeHighscores();

	optionFocused = 0;

	textShader->use();
	textShader->setMatrix4("projection", projection);

	options.push_back(MenuOption( ));
	options.push_back(MenuOption( ));
	options.push_back(MenuOption( ));
	options.push_back(MenuOption( ));

	options.at(0).setLabel("Play", *font);
	options.at(1).setLabel("Options", *font);
	options.at(2).setLabel("HighScores", *font);
	options.at(3).setLabel("Exit", *font);

	options.at(0).setcenterPosition( glm::vec2( 960 , 800 ) );
	options.at(1).setcenterPosition(glm::vec2(960 , 600));
	options.at(2).setcenterPosition(glm::vec2(960 , 400));
	options.at(3).setcenterPosition(glm::vec2(960, 200));
}

void GameMenu::render()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	textShader->use();
	textShader->setMatrix4("projection", projection);

	switch (menuOptionSelected)
	{
	case MENU:
	{
		for (int i = 0; i < options.size(); i++)
		{
			options.at(i).draw(*font, *textShader);
		}
		break;
	}
	case OPTIONS:
		break;
	case HIGHSCORES:
	{
		for (size_t i = 0; i < 10; i++)
		{
			font->RenderText(*textShader, highscores.getHighscoresText(i), 900, 900 - i * 80, 1.0f, glm::vec3(0.0, 0.0, 0.0));
		}
		break;
	}
	case GAME:
		break;
	default:
		break;
	}




}
void GameMenu::update(float deltaTime)
{
	options.at(optionFocused).updateScale(deltaTime);
}

void GameMenu::initializeHighscores()
{
	highscores.loadFromFile();
}

void GameMenu::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void GameMenu::mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset)
{

}
void GameMenu::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (menuOptionSelected == GameState::MENU)
	{


		if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
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
				menuOptionSelected = GameState::HIGHSCORES;
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
	else
	{
		highscoresKeyEvents(window, key, scancode, action, mods);
	}
}

void GameMenu::highscoresKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
		menuOptionSelected = GameState::MENU;
	}
}

void GameMenu::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{

}
void GameMenu::setPostProcessEffect(POST_PROCESSING_EFFECT effect)
{

}