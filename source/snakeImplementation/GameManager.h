#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../glfw/Glfw.h"

#include "../engine/macros.h"

#include "../engine/files/InitializationFile.h"

class Engine;
class Game;
class GameMenu;
enum POST_PROCESSING_EFFECT;

enum GameState
{
	MENU,
	OPTIONS,
	HIGHSCORES,
	GAME,
};

class GameManager
{
	GameMenu* gameMenu;
	Game* game;

	GameState state;

	InitializationFile optionsFile;


public:
	GameManager(Engine* engine);
	~GameManager();

	void render();
	void update(float deltaTime);

	void switchToMenu();
	void switchToGame();

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	 void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	void setPostProcessEffect(POST_PROCESSING_EFFECT effect);
	
	GETTER(Game*, game);
	GETTER(GameMenu*, gameMenu);
	GETTER(GameState, state);
	GETTER_REF(InitializationFile, optionsFile);
	SETTER(GameState, state);
};

#endif