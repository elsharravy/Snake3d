#ifndef HIGHSCORESCREEN_H
#define HIGHSCORESCREEN_H

#include "../glfw/Glfw.h"
#include "Options/OptionsScreen.h"

class GameMenu;

class HighscoreScreen : public OptionsScreen 
{

public:
	using OptionsScreen::OptionsScreen; 

	virtual void render();

	virtual void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif