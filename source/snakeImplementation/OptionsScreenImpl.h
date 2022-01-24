#ifndef OPTIONSSCREENIMPL_H
#define OPTIONSSCREENIMPL_H

#include "Options/OptionsScreen.h"

class OptionsScreenImpl : public OptionsScreen
{

	int choosenDifficulty;
	int difficultyMIN;
	int difficultyMAX;

	int choosenMouseSensitivity;
	float minimumMouseSensitivity;
	float mouseSensitivityStep;
	int mouseSensitivityMIN;
	int mouseSensitivityMAX;

	void loadDifficultyFromFile();
	void loadMouseSensitivityFromFile();

public:
//	using OptionsScreen::OptionsScreen;
	OptionsScreenImpl(GameMenu* menu);

	virtual void render();
	virtual void update(float deltaTime);

	void saveOptions();

	virtual void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif