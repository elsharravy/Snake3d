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

	int keyForMovingMinusZAxis;
	int keyForMovingPlusZAxis;
	int keyForMovingMinusXAxis;
	int keyForMovingPlusXAxis;

	Board board;

	// used for detecting when user release 'P' key
	bool isKeyP_pressed;

	Snake snake;

	bool pause;

	glm::vec4 clearColor;

	GLFWwindow* window;

	Camera cam;

	glm::vec3 foodPos;

	ShaderProgram colorShader;

	double lastFrameCursorPosX;
	double lastFrameCursorPosY;

	float deltaTime;
	float lastFrame;

	void compileAndLinkShader(ShaderProgram* shader, const std::string& vertexShaderPath , const std::string& fragmentShaderPath);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void updateViewMatrixInShaders();

	void drawBorders(Line& line1, Line& line2, Line& line3, ShaderProgram& shader);

	void processInput();

	void render();

	void setMovingKeys( float angle );

	void generateRandomFood();

public:
	Engine(GLFWwindow* window);

	static GLFWwindow* createWindow(int width, int height, const std::string& title);

	void start();


};


#endif
