#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "../glfw/Glfw.h"

#include "../engine/shaders/shaderProgram.h"
#include "../engine/fonts/Fonts.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

class GameManager;
class Engine;
enum POST_PROCESSING_EFFECT;

class GameMenu
{
	GameManager* gameManager;
	Engine* engine;
	Font font;
	ShaderProgram textShader;

	glm::mat4 projection;
public:
	GameMenu(GameManager* gameManager, Engine* engine);
	
	void render();
	void update(float deltaTime);

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	void setPostProcessEffect(POST_PROCESSING_EFFECT effect);
};

#endif