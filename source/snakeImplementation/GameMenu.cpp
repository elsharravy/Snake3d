#include "GameMenu.h"

#include "Game.h"

void GameMenu::render()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
void GameMenu::update(float deltaTime)
{

}


void GameMenu::mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset)
{

}
void GameMenu::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
void GameMenu::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{

}
void GameMenu::setPostProcessEffect(POST_PROCESSING_EFFECT effect)
{

}