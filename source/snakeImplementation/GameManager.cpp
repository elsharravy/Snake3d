#include "GameManager.h"

#include "GameMenu.h"
#include "Game.h"

#include "../engine/resources/ResourceLoader.h"

#define OPTIONS_FILE_PATH "resources/data/options.ini"

GameManager::GameManager(Engine* engine) : state ( GameState::MENU ), optionsFile(OPTIONS_FILE_PATH)
{
	ResourceLoader::loadResources();

	optionsFile.populateValues();

	game = new Game(engine, this);
	gameMenu = new GameMenu(this, engine);

}

GameManager::~GameManager()
{
	delete game;
	delete gameMenu;
}


void GameManager::render()
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->render();
		break;
	case GameState::GAME:
		game->render();
		break;
	default:
		break;
	}
}
void GameManager::update(float deltaTime)
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->update(deltaTime);
		break;
	case GameState::GAME:
		game->update(deltaTime);
		break;
	default:
		break;
	}
}

void GameManager::mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset)
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->mouseMovedEvent(window, xpos, ypos , xoffset, yoffset);
		break;
	case GameState::GAME:
		game->mouseMovedEvent(window, xpos, ypos, xoffset, yoffset);
		break;
	default:
		break;
	}
}

void GameManager::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->keyEvent(window, key, scancode, action, mods);
		break;
	case GameState::GAME:
		game->keyEvent(window, key, scancode, action, mods);
		break;
	default:
		break;
	}
}

void GameManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->mouse_button_callback(window, button, action, mods);
		break;
	case GameState::GAME:
		game->mouse_button_callback(window, button, action, mods);
		break;
	default:
		break;
	}
}

void GameManager::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->scrollEvent( window,  xoffset,  yoffset);
		break;
	case GameState::GAME:
		game->scrollEvent(window, xoffset, yoffset);
		break;
	default:
		break;
	}
}

void GameManager::setPostProcessEffect(POST_PROCESSING_EFFECT effect)
{
	switch (state)
	{
	case GameState::MENU:
		gameMenu->setPostProcessEffect(effect);
		break;
	case GameState::GAME:
		game->setPostProcessEffect(effect);
	default:
		break;
	}
}