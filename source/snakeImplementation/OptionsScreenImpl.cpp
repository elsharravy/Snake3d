#include "OptionsScreenImpl.h"

#include "GameMenu.h"
#include "GameManager.h"

#include "../engine/resources/Sounds.h"

#define SELECTED_OPTION_COLOR glm::vec3(0.47f, 0.0f, 0.0f)
#define UNSELECTED_OPTION_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

OptionsScreenImpl::OptionsScreenImpl(GameMenu* menu) : OptionsScreen( menu )
{
	loadDifficultyFromFile();
	loadMouseSensitivityFromFile();

	options.push_back(MenuOption());
	options.push_back(MenuOption());
	options.push_back(MenuOption());

	options.at(0).setLabel("Back" , *font);
	options.at(1).setLabel("Difficulty: " + std::to_string(choosenDifficulty), *font);
	options.at(2).setLabel("Camera sensitivity: " + std::to_string(choosenMouseSensitivity), *font);

	options.at(0).setcenterPosition(glm::vec2(960, 800));
	options.at(1).setcenterPosition(glm::vec2(960, 700));
	options.at(2).setcenterPosition(glm::vec2(960, 600));

	options.at(0).setcolor(SELECTED_OPTION_COLOR);
}

void OptionsScreenImpl::loadDifficultyFromFile()
{
	choosenDifficulty = atoi(menu->getgameManager()->getoptionsFile().getValue("settings", "difficulty").c_str());
	difficultyMIN = atoi(menu->getgameManager()->getoptionsFile().getValue("settings", "difficultyMIN").c_str());
	difficultyMAX = atoi(menu->getgameManager()->getoptionsFile().getValue("settings", "difficultyMAX").c_str());
}

void OptionsScreenImpl::loadMouseSensitivityFromFile()
{
	minimumMouseSensitivity = menu->getgameManager()->getoptionsFile().getFloat("settings", "mouseSensitivityMIN");
	mouseSensitivityMIN = 0;
	mouseSensitivityMAX = menu->getgameManager()->getoptionsFile().getInt("settings", "mouseSensitivitySteps");

	float sensitivityMax = menu->getgameManager()->getoptionsFile().getFloat("settings", "mouseSensitivityMAX");
	float inFileSensitivity = menu->getgameManager()->getoptionsFile().getFloat("settings", "mouseSensitivity");

	 mouseSensitivityStep = (( sensitivityMax - minimumMouseSensitivity ) / mouseSensitivityMAX);

	 choosenMouseSensitivity = ((inFileSensitivity - minimumMouseSensitivity) / mouseSensitivityStep);
}

void OptionsScreenImpl::saveOptions()
{
	menu->getgameManager()->getoptionsFile().setValue("settings", "difficulty", choosenDifficulty);
	menu->getgameManager()->getoptionsFile().setValue("settings", "mouseSensitivity", minimumMouseSensitivity + mouseSensitivityStep * choosenMouseSensitivity);

	menu->getgameManager()->getoptionsFile().saveValuesToFile();
}

void OptionsScreenImpl::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
		playClick();

		switch (optionFocused)
		{
		case 0:
		{
			saveOptions();
			menu->setmenuOptionSelected(GameState::MENU);	
			break;
		}
		case 1:
		{
			choosenDifficulty += 1;
			if (choosenDifficulty > difficultyMAX)
			{
				choosenDifficulty = difficultyMIN;
			}
			options.at(1).setLabel("Difficulty: " + std::to_string(choosenDifficulty), *font);
			break;
		}
		case 2:
		{
			choosenMouseSensitivity += 1;
			if (choosenMouseSensitivity > mouseSensitivityMAX)
			{
				choosenMouseSensitivity = mouseSensitivityMIN;
			}
			options.at(2).setLabel("Camera sensitivity: " + std::to_string(choosenMouseSensitivity), *font);
			break;
		}
		case 3:
		{
	//		engine->requestEngineClose();
			break;
		}
		default:
			break;
		}
		//	gameManager->setstate(GameState::GAME);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		playSwitch();

		options.at(optionFocused).setcolor(UNSELECTED_OPTION_COLOR);
		options.at(optionFocused).resetScale();

		optionFocused += 1;

		if (optionFocused >= options.size())
		{
			optionFocused = 0;
		}

		options.at(optionFocused).setcolor(SELECTED_OPTION_COLOR);
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		playSwitch();

		options.at(optionFocused).setcolor(UNSELECTED_OPTION_COLOR);
		options.at(optionFocused).resetScale();

		optionFocused -= 1;

		if (optionFocused < 0)
		{
			optionFocused = (options.size() - 1);
		}

		options.at(optionFocused).setcolor(SELECTED_OPTION_COLOR);
	}
}

void OptionsScreenImpl::playClick()
{
	Sounds::playSound("resources/sounds/click.mp3");
}

void OptionsScreenImpl::playSwitch()
{
	Sounds::playSound("resources/sounds/change.mp3");
}

void OptionsScreenImpl::render()
{
	for (int i = 0; i < options.size(); i++)
	{
		options.at(i).draw(*font, *textShader);
	}
}

void OptionsScreenImpl::update(float deltaTime)
{
	options.at(optionFocused).updateScale(deltaTime);
}
