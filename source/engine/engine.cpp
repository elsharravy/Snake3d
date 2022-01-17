#include "Engine.h"

#include "../Glad/Glad.h"

#include "primitives/triangle.h"
#include "primitives/cube.h"
#include "primitives/line.h"
#include "primitives/Point3d.h"
#include "colors/Color.h"
#include "texture/texture.h"
#include "Random/Random.h"
#include "lightning/light.h"
#include "lightning/material.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "macros.h"
#include "models/model.h"

#include "../stb_image/stb_image.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include <iostream>

Engine::Engine(GLFWwindow* window) : window(window),  clearColor(Color::White),  yAxis(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f)),
zAxis(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 100.0f)),
xAxis(glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f))
{
	callbacksHelperEngine = this;

	initializeAxes();

	glfwGetCursorPos(window, &lastFrameCursorPosX, &lastFrameCursorPosY);
	
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
}
Engine::~Engine()
{
	delete game;
}

void Engine::compileAndLinkShader(ShaderProgram* shader, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	Shader vShader(GL_VERTEX_SHADER);

	std::string vShaderCompileMessage = vShader.compileFromPath(vertexShaderPath);

	if (vShaderCompileMessage.size() > 0)
	{
		LOG(vertexShaderPath + " : " + vShaderCompileMessage);
	}
	else
	{
		LOG(vertexShaderPath + " : " + "vertex shader compiled without errors");
	}

	Shader fShader(GL_FRAGMENT_SHADER);

	std::string fShaderCompileMessage = fShader.compileFromPath(fragmentShaderPath);

	if (fShaderCompileMessage.size() > 0)
	{
		LOG(fragmentShaderPath + " : " + fShaderCompileMessage);
	}
	else
	{
		LOG(fragmentShaderPath + " : " + "fragment shader compiled without errors");
	}

	shader->link(&vShader, &fShader);
}

GLFWwindow* Engine::createWindow(int width, int height, const std::string& title)
{
	GLFWwindow* window = Glfw::createWindow(width, height, title);
	return window;
}

void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
		float xoffset = xpos - callbacksHelperEngine->lastFrameCursorPosX;
		float yoffset = callbacksHelperEngine->lastFrameCursorPosY - ypos; // reversed since y-coordinates range from bottom to top

		//updating cursor positions
		callbacksHelperEngine->lastFrameCursorPosX = xpos;
		callbacksHelperEngine->lastFrameCursorPosY = ypos;

		callbacksHelperEngine->game->mouseMovedEvent(window, xpos, ypos, xoffset, yoffset);
}

void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	callbacksHelperEngine->game->keyEvent(window, key, scancode, action, mods);
}

void Engine::drawAxes()
{
	xAxis.draw( game->getlightSourceShader() );
	yAxis.draw(game->getlightSourceShader());
	zAxis.draw(game->getlightSourceShader());
}

void Engine::initializeAxes()
{
	xAxis.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	yAxis.setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	zAxis.setColor(glm::vec3(0.0f, 1.0f, 0.0f));

	xAxis.setWidth(5);
	yAxis.setWidth(5);
	zAxis.setWidth(5);
}

void Engine::update()
{
	game->update(deltaTime);
}

void Engine::render()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
//	drawAxes();

	game->render();

	glfwSwapBuffers(window);
}


void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	callbacksHelperEngine->game->scrollEvent(window, xoffset, yoffset);
}

void Engine::start()
{
	LOG("initialization start");

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_PROGRAM_POINT_SIZE);	// enabling changing point size in shaders
	
	int windowY;
	int windowX;
	glfwGetWindowSize(window, &windowX, &windowY);
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowX) / windowY, 0.1f, 100.0f);

	game = new Game(this);

	game->updateProjectionMatrixInShaders(projection);
	
	LOG("initializing end");

	lastFrame = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		update();
		render();

		glfwPollEvents();
	}
}

void Engine::requestEngineClose()
{
	glfwSetWindowShouldClose(window, true);
}


Engine* Engine::callbacksHelperEngine = NULL;