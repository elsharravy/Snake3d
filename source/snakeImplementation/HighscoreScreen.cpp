#include "HighscoreScreen.h"

#include "GameMenu.h"
#include "GameManager.h"


void HighscoreScreen::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
	menu->setmenuOptionSelected(GameState::MENU);
	}
}

void HighscoreScreen::render()
{
	for (size_t i = 0; i < 10; i++)
	{
		font->RenderText(*textShader, menu->gethighscores().getHighscoresText(i), 900, 900 - i * 80, 1.0f, glm::vec3(0.0, 0.0, 0.0));
	}
}

