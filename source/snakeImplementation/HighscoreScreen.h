#ifndef HIGHSCORESCREEN_H
#define HIGHSCORESCREEN_H

#include "../glfw/Glfw.h"
#include "Options/OptionsScreen.h"

#include <vector>
#include <glm/vec3.hpp>

class GameMenu;

class HighscoreScreen : public OptionsScreen 
{
	std::vector<glm::vec3> colors;

	int scoreWithDifferentColor;
public:
//	using OptionsScreen::OptionsScreen; 
	HighscoreScreen(GameMenu* menu);

	void setDifferentColor(int scoreWithDifferentColor);

	virtual void render();

	virtual void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif