#include "OptionsScreenImpl.h"

#include "GameMenu.h"
#include "GameManager.h"


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
		options.at(optionFocused).resetScale();

		optionFocused += 1;

		if (optionFocused >= options.size())
		{
			optionFocused = 0;
		}
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		options.at(optionFocused).resetScale();

		optionFocused -= 1;

		if (optionFocused < 0)
		{
			optionFocused = (options.size() - 1);
		}
	}
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
