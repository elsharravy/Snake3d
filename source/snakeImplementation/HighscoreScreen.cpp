#include "HighscoreScreen.h"

#include "GameMenu.h"
#include "GameManager.h"

#define HIGHSCORE_TEXT_COLOR glm::vec3( 0.0f, 0.0f, 0.0f )
#define HIGHSCORE_DIFFERENT_TEXT_COLOR glm::vec3( 1.0f, 0.0f, 0.0f )

HighscoreScreen::HighscoreScreen(GameMenu* menu) : OptionsScreen(menu)
{
	for (size_t i = 0; i < 10; i++)
	{
		colors.push_back(HIGHSCORE_TEXT_COLOR);
	}
}

void HighscoreScreen::setDifferentColor(int scoreWithDifferentColor)
{
	this->scoreWithDifferentColor = scoreWithDifferentColor;
	colors.at(scoreWithDifferentColor) = HIGHSCORE_DIFFERENT_TEXT_COLOR;
}

void HighscoreScreen::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
	menu->setmenuOptionSelected(GameState::MENU);
	colors.at(scoreWithDifferentColor) = HIGHSCORE_TEXT_COLOR;
	}
}

void HighscoreScreen::render()
{
	for (size_t i = 0; i < 10; i++)
	{
		font->RenderText(*textShader, menu->gethighscores().getHighscoresText(i), 900, 900 - i * 80, 1.0f, colors.at(i));
	}
}

