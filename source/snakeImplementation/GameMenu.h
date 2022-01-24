#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "../glfw/Glfw.h"

#include "../engine/shaders/shaderProgram.h"
#include "../engine/fonts/Fonts.h"
#include "Options/MenuOption.h"
#include "HighscoreScreen.h"
#include "OptionsScreenImpl.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "HighScores.h"

#include "../engine/macros.h"

#include <vector>

class GameManager;
class Engine;
enum POST_PROCESSING_EFFECT;
enum GameState;


class GameMenu
{
	GameManager* gameManager;
	Engine* engine;
	Font* font;
	ShaderProgram* textShader;

	HighScores highscores;

	HighscoreScreen highscoresScreen;
	OptionsScreenImpl optionsScreen;

	GameState menuOptionSelected;

	int optionFocused;

	std::vector< MenuOption > options;

	glm::mat4 projection;

	void initializeHighscores();

public:
	GameMenu(GameManager* gameManager, Engine* engine);
	
	void render();
	void update(float deltaTime);

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void setPostProcessEffect(POST_PROCESSING_EFFECT effect);

	GETTER_REF(HighScores, highscores);
	GETTER(GameManager*, gameManager);
	SETTER(GameState, menuOptionSelected);
};

#endif