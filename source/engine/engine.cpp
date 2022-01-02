#include "Engine.h"

#include "../Glad/Glad.h"

#include "primitives/triangle.h"
#include "primitives/cube.h"
#include "primitives/line.h"
#include "primitives/Point3d.h"
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

#define SNAKE_VELOCITY 5

Engine::Engine(GLFWwindow* window) : window(window), lastFrameCursorPosX(400), lastFrameCursorPosY(300), firstMouseInput(true), clearColor(1.0, 1.0, 1.0, 0.0), snake( &board )
{
	callbacksHelperEngine = this;

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	isPpressed = false;
	pause = false;
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
	if (callbacksHelperEngine->firstMouseInput) // initially set to true
	{
		callbacksHelperEngine->lastFrameCursorPosX = xpos;
		callbacksHelperEngine->lastFrameCursorPosY = ypos;
		callbacksHelperEngine->firstMouseInput = false;
	}
	else
	{
		float xoffset = xpos - callbacksHelperEngine->lastFrameCursorPosX;
		float yoffset = callbacksHelperEngine->lastFrameCursorPosY - ypos; // reversed since y-coordinates range from bottom to top

		callbacksHelperEngine->lastFrameCursorPosX = xpos;
		callbacksHelperEngine->lastFrameCursorPosY = ypos;

//		callbacksHelperEngine->cam.updatePitchAndYaw(yoffset, xoffset);

		if (glm::abs(xoffset) > glm::abs(yoffset))
		{
			callbacksHelperEngine->cam.addAngle(-xoffset);
		}
		else
		{
			callbacksHelperEngine->cam.addAngleY(yoffset);
		}

		callbacksHelperEngine->updateShadersViewMatrix();
	}

}

void Engine::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
//		cam.move(glm::vec3(0.0f, 0.0f, -0.1f));
//		cam.moveForward();
		updateShadersViewMatrix();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		//cam.move(glm::vec3(0.0f, 0.0f, 0.1f));
//		cam.moveBackward();
		updateShadersViewMatrix();
	}	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
//		cam.move(glm::vec3(-0.1f, 0.0f, 0.0f));
		updateShadersViewMatrix();
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
//		cam.move(glm::vec3(0.1f, 0.0f, 0.0f));
		updateShadersViewMatrix();
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		isPpressed = true;
	}
	else
	{
		if ( isPpressed )
		{
			pause = !pause;
		}
		isPpressed = false;
	}

	// moving snake

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		snake.requestDirectionChange(glm::ivec3(0, 0, -1) , glm::ivec3(0, 0, 1));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		snake.requestDirectionChange(glm::ivec3(0, 0, 1), glm::ivec3(0, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		snake.requestDirectionChange(glm::ivec3(-1, 0, 0), glm::ivec3(1, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		snake.requestDirectionChange(glm::ivec3(1, 0, 0), glm::ivec3(0, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		snake.requestDirectionChange(glm::ivec3(0, 1, 0), glm::ivec3(0, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		snake.requestDirectionChange(glm::ivec3(0, -1, 0), glm::ivec3(0, 1, 0));
	}
}

void Engine::render()
{
	glClearColor(clearColor.R, clearColor.G, clearColor.B, clearColor.A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void Engine::start()
{
	LOG("initialization start");

	
	//for (int i = 0; i < 10; i++)
	//{
	//	board.setFieldState(i, 0, i, Field::SNAKE);
	//}
	generateRandomFood();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

//	cam.move(glm::vec3(5.0f, 21.0f, 16.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	Line yAxis(glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 100.0f, 0.0f));
	Line zAxis(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 100.0f));
	Line xAxis(glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(100.0f, 0.0f, 0.0f));

	xAxis.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	yAxis.setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	zAxis.setColor(glm::vec3(0.0f, 1.0f, 0.0f));

	xAxis.setWidth(5);
	yAxis.setWidth(5);
	zAxis.setWidth(5);

	Line border(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f));
	Line border2(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
	Line border3(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f));

	border.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
	border2.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
	border3.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
	

	Cube snakeSegment(1, 1, 1);
	snakeSegment.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	snakeSegment.setTextureCoords(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(1.0f, 1.0f), Vec2f(0.0f, 1.0f));
//	snakeSegment.move( glm::vec3(0.0, 0.0, 0.0) );

	Cube snakeFood(1, 1, 1);
	snakeFood.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	snakeFood.setTextureCoords(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.0f), Vec2f(1.0f, 1.0f), Vec2f(0.0f, 1.0f));
	snakeFood.setPosition(foodPos);

	compileAndLinkShader(&shaderP, "resources/shaders/vShader.vs", "resources/shaders/fShader.fs");
	compileAndLinkShader(&colorShader, "resources/shaders/colorShader.vs", "resources/shaders/colorShader.fs");
	compileAndLinkShader(&backShader, "resources/shaders/back.vs", "resources/shaders/back.fs");

	updateShadersViewMatrix();

	shaderP.use();

//	shaderP.setMatrix4("view", cam.getView() );

	shaderP.setMatrix4("projection", projection);

	colorShader.use();
//	colorShader.setMatrix4("view", cam.getView());
	colorShader.setMatrix4("projection", projection);

	backShader.use();

	backShader.setMatrix4("projection", projection);

	snake.setSnakeVelocity(SNAKE_VELOCITY);
	
	LOG("initializing end");

	lastFrame = glfwGetTime();
	
	while (!glfwWindowShouldClose(window))
	{
//		cout << cam.getAngleY() << endl;

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		shaderP.use();

		shaderP.setCameraPos(cam.getPosition());

		processInput();

		render();

		//xAxis.draw(colorShader);
		//yAxis.draw(colorShader);
		//zAxis.draw(colorShader);

		drawBorders(border, border2, border3, colorShader);

		if (!pause)
		{
			Field currentField = snake.move(deltaTime);

			if (currentField == Field::SNAKE)	// we hit snake body, we lose
			{
				glfwSetWindowShouldClose(window, true);
			}
			else if(currentField == Field::FOOD)	// we eat food, we need another in random position in the world
			{
				generateRandomFood();
			}
			else if (currentField == Field::WALL)	// we eat food, we need another in random position in the world
			{
				cout << "Hit the wall" << endl;
				glfwSetWindowShouldClose(window, true);
			}
		}

		snake.draw(snakeSegment, colorShader);

		snakeFood.setPosition(foodPos);
		snakeFood.draw(colorShader);


		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

void Engine::updateShadersViewMatrix()
{
	shaderP.use();
	shaderP.setMatrix4("view", cam.getView());
	colorShader.use();
	colorShader.setMatrix4("view", cam.getView());
	backShader.use();
	backShader.setMatrix4("view", cam.getView());

}

void Engine::drawBorders(Line& line1, Line& line2, Line& line3 , ShaderProgram& shader)
{
	// drawing border

	line1.setPosition(glm::vec3(0, 0, 0));
	line2.setPosition(glm::vec3(0, 0, 0));
	line3.setPosition(glm::vec3(0, 0, 0));

	line1.draw(shader);
	line2.draw(shader);
	line3.draw(shader);

	line1.setPosition(glm::vec3(0, 0, 10));
	line2.setPosition(glm::vec3(10, 0, 0));
	line3.setPosition(glm::vec3(10, 0, 0));

	line1.draw(shader);
	line2.draw(shader);
	line3.draw(shader);

	line1.setPosition(glm::vec3(0, 10, 10));
	line2.setPosition(glm::vec3(10, 0, 10));
	line3.setPosition(glm::vec3(10, 10, 0));

	line1.draw(shader);
	line2.draw(shader);
	line3.draw(shader);

	line1.setPosition(glm::vec3(0, 10, 0));
	line2.setPosition(glm::vec3(0, 0, 10));
	line3.setPosition(glm::vec3(0, 10, 0));

	line1.draw(shader);
	line2.draw(shader);
	line3.draw(shader);
}

void Engine::generateRandomFood()
{
	do
	{
		foodPos = glm::ivec3(Random::randInt(0, BOARD_SIDE_SIZEX), Random::randInt(0, BOARD_SIDE_SIZEY), Random::randInt(0, BOARD_SIDE_SIZEZ));
	} while (board.getFieldState(foodPos) == Field::SNAKE);
	
	board.setFieldState(foodPos, Field::FOOD);
}

Engine* Engine::callbacksHelperEngine = NULL;