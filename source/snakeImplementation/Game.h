#ifndef GAME_H
#define GAME_H

#include "../engine/primitives/line.h"
#include "../engine/primitives/cube.h"
#include "../engine/camera/camera.h"
#include "../engine/shaders/shaderProgram.h"
#include "../engine/cubemap/cubemap.h"
#include "Snake.h"
#include "board.h"
#include "../engine/macros.h"

class Engine;

class Game
{
	Engine* engine;

	ShaderProgram colorShader;
	ShaderProgram lightSourceShader;
	ShaderProgram cubemapShader;

	Board board;
	Snake snake;
	Camera cam;

	Cubemap skyBox;
	Cube skyBoxCube;

	bool pause;

	Line border, border2, border3;
	Cube* snakeSegment,* snakeFood;

	Cube lightSource;

	int keyForMovingMinusZAxis;
	int keyForMovingPlusZAxis;
	int keyForMovingMinusXAxis;
	int keyForMovingPlusXAxis;

	glm::vec3 foodPos;

	void initializeSkyBox();
	void initializeCubeBorders();
	void initializeSnake();
	void initializeKeySettings();
	void initializeSnakeSegment();
	void initializeSnakeFood();
	void initializeShaders();

	void setMovingKeys(float angle);
	void generateRandomFood();

	void drawBorders(ShaderProgram& shader);
	void updateViewMatrixInShaders();
public:
	Game(Engine* engine);
	~Game();

	void render();
	void update( float deltaTime );

	void gameOver();

	void renderSkyBox();

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);

	void updateProjectionMatrixInShaders(glm::mat4 projection);

	GETTER_REF(ShaderProgram, lightSourceShader);
};

#endif