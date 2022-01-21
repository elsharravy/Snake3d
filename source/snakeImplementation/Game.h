#ifndef GAME_H
#define GAME_H

#include "../engine/primitives/line.h"
#include "../engine/primitives/cube.h"
#include "../engine/primitives/NDCquad.h"
#include "../engine/camera/camera.h"
#include "../engine/shaders/shaderProgram.h"
#include "../engine/cubemap/cubemap.h"
#include "../engine/framebuffer/framebuffer.h"
#include "Snake.h"
#include "board.h"
#include "../engine/macros.h"

class Engine;
class GameManager;


enum POST_PROCESSING_EFFECT
{
	NONE = 0,
	COLOR_INVERSION = 1,
	GRAYSCALE = 2,
	KERNEL = 3,
};

class Game
{
	Engine* engine;
	GameManager* gameManager;

	// shaders
	ShaderProgram colorShader;
	ShaderProgram lightSourceShader;
	ShaderProgram cubemapShader;
	ShaderProgram postProcessShader;

	Board board;
	Snake snake;
	Camera cam;

	// skybox
	Cubemap skyBox;
	Cube skyBoxCube;

	// post processing
	Framebuffer postProcessFramebuffer;
	NDCquad postProcessScreen;
	POST_PROCESSING_EFFECT activePostProcessEffect;

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
	void initializePostProcessing();

	void resetGame();

	void setMovingKeys(float angle);
	void generateRandomFood();

	void drawBorders(ShaderProgram& shader);
	void updateViewMatrixInShaders();
public:
	Game(Engine* engine, GameManager* gameManager);
	~Game();

	void render();
	void update( float deltaTime );

	void gameOver();

	void renderSkyBox();

	void mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset);
	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


	void setPostProcessEffect(POST_PROCESSING_EFFECT effect);

	void updateProjectionMatrixInShaders(glm::mat4 projection);

	GETTER_REF(ShaderProgram, lightSourceShader);
};

#endif