#include "GameManager.h"

#include "GameMenu.h"
#include "Game.h"

GameManager::GameManager(Engine* engine) : state ( GameState::GAME )
{
	game = new Game(engine);
	gameMenu = new GameMenu();
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
	case GameManager::MENU:
		gameMenu->render();
		break;
	case GameManager::GAME:
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
	case GameManager::MENU:
		gameMenu->update(deltaTime);
		break;
	case GameManager::GAME:
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
	case GameManager::MENU:
		gameMenu->mouseMovedEvent(window, xpos, ypos , xoffset, yoffset);
		break;
	case GameManager::GAME:
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
	case GameManager::MENU:
		gameMenu->keyEvent(window, key, scancode, action, mods);
		break;
	case GameManager::GAME:
		game->keyEvent(window, key, scancode, action, mods);
		break;
	default:
		break;
	}
}

void GameManager::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	switch (state)
	{
	case GameManager::MENU:
		gameMenu->scrollEvent( window,  xoffset,  yoffset);
		break;
	case GameManager::GAME:
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
	case GameManager::MENU:
		gameMenu->setPostProcessEffect(effect);
		break;
	case GameManager::GAME:
		game->setPostProcessEffect(effect);
	default:
		break;
	}
}