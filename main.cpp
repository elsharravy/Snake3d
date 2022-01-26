#include "source/Glad/Glad.h"
#include "source/glfw/Glfw.h"

#include "source/engine/engine.h"
#include "source/engine/macros.h"

#include "source/stb_image/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <iostream>

#define MAJOR_OPENGL_VERSION 3
#define MINOR_OPENGL_VERSION 3

#define WINDOW_X 1920
#define WINDOW_Y 1080

//#define WINDOW_X 1366
//#define WINDOW_Y 768

#define WINDOW_TITLE "Snake 3d"

int main()
{
//	stbi_set_flip_vertically_on_load(true);

	bool GLFWResult = Glfw::initGlfw(MAJOR_OPENGL_VERSION, MINOR_OPENGL_VERSION, GLFW_OPENGL_CORE_PROFILE);
	if (!GLFWResult)
	{
		LOG("Initializing GLFW error");
		return ERROR_EXIT_CODE;
	}

	glm::vec2 screenSize = Engine::getWindowsScreenSize();

	GLFWwindow* window = Engine::createWindow(screenSize.x, screenSize.y, WINDOW_TITLE, glfwGetPrimaryMonitor());
//	GLFWwindow* window = Engine::createWindow(WINDOW_X, WINDOW_Y, WINDOW_TITLE, glfwGetPrimaryMonitor());

	if (window == NULL)
	{
		LOG("Failed to initialize window");
		return ERROR_EXIT_CODE;
	}

	bool gladInitialization = Glad::loadFunctionPointers();
	if (!gladInitialization)
	{
		LOG("Glad initialization error");
		return ERROR_EXIT_CODE;
	}

	Engine engine(window);

	engine.start();

	return 0;
}