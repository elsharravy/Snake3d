#ifndef GAME_H
#define GAME_H

#include "../engine/primitives/line.h"
#include "../engine/primitives/cube.h"
#include "../engine/camera/camera.h"
#include "../engine/shaders/shaderProgram.h"
#include "Snake.h"
#include "board.h".h"

class Engine;

class Game
{
	Engine* engine;

	Board board;
	Snake snake;
	Camera cam;

	bool pause;

	Line border, border2, border3;
	Cube* snakeSegment,* snakeFood;

	int keyForMovingMinusZAxis;
	int keyForMovingPlusZAxis;
	int keyForMovingMinusXAxis;
	int keyForMovingPlusXAxis;

	glm::vec3 foodPos;

	void initializeSnake();
	void initializeKeySettings();
	void initializeSnakeSegment();
	void initializeSnakeFood();

	void setMovingKeys(float angle);
	void generateRandomFood();

	void drawBorders(ShaderProgram& shader);
	void updateViewMatrixInShaders();
public:
	Game(Engine* engine);
	~Game();

	void render(ShaderProgram& shader);
	void update( float deltaTime );

	void gameOver();

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);

	void updateProjectionMatrixInShaders(glm::mat4 projection);
};

#endif