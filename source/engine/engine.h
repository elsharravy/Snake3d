#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "../glfw/Glfw.h"

#include "primitives/line.h"
#include "../snakeImplementation/board.h"
#include "../snakeImplementation/snakeHead.h"
#include "../snakeImplementation/Snake.h"
#include "camera/camera.h"
#include "colors/color.h"
#include "shaders/shaderProgram.h"

class Engine
{
	static Engine* callbacksHelperEngine;

	Board board;

	bool isPpressed;

	Snake snake;

	bool pause;

	Color clearColor;

	GLFWwindow* window;

	Camera cam;

	glm::vec3 foodPos;

	ShaderProgram shaderP;
	ShaderProgram colorShader;
	ShaderProgram backShader;

	bool firstMouseInput;

	float lastFrameCursorPosX;
	float lastFrameCursorPosY;

	float deltaTime;
	float lastFrame;

	void compileAndLinkShader(ShaderProgram* shader, const std::string& vertexShaderPath , const std::string& fragmentShaderPath);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void updateShadersViewMatrix();
//	void updateShadersProjectionMatrix();

	void drawBorders(Line& line1, Line& line2, Line& line3, ShaderProgram& shader);

	void processInput();
	void render();

	void generateRandomFood();

public:
	Engine(GLFWwindow* window);

	static GLFWwindow* createWindow(int width, int height, const std::string& title);

	void start();


};


#endif
