#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "../glfw/Glfw.h"

enum POST_PROCESSING_EFFECT;

class GameMenu
{

public:
	void render();
	void update(float deltaTime);

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	void setPostProcessEffect(POST_PROCESSING_EFFECT effect);
};

#endif