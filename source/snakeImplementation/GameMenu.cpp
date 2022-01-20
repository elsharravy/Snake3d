#include "GameMenu.h"

#include "Game.h"
#include "GameManager.h"


#include "../engine/engine.h"

GameMenu::GameMenu(GameManager* gameManager,Engine* engine) : gameManager(gameManager), engine(engine), font("resources/fonts/arial.ttf", 48)
{
	engine->compileAndLinkShader(&textShader, "resources/shaders/text/textShader.vs", "resources/shaders/text/textShader.fs");
	projection = glm::ortho(0.0f, 1900.0f, 0.0f, 1080.0f);
	font.loadCharacters();
	
	textShader.use();
	textShader.setMatrix4("projection", projection);
}

void GameMenu::render()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	font.RenderText(textShader, "Finally", 100, 100, 1.0f , glm::vec3( 0.0, 0.0 , 0.0 ));

}
void GameMenu::update(float deltaTime)
{

}


void GameMenu::mouseMovedEvent(GLFWwindow* window, double xpos, double ypos, double xoffset, double yoffset)
{

}
void GameMenu::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_M && action == GLFW_RELEASE)
	{
	gameManager->setstate(GameState::GAME);
	}
}
void GameMenu::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{

}
void GameMenu::setPostProcessEffect(POST_PROCESSING_EFFECT effect)
{

}