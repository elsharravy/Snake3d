#include "HighScores.h"

#include <algorithm>

HighScores::HighScores(std::string filePath) : filePath(filePath)
{

}

void HighScores::loadFromFile()
{
	file.open(filePath, std::ios_base::in);

	int score;

	for (size_t i = 0; i < 10; i++)
	{
		file >> score;
		highscores.push_back(score);
	}

	highscoresText.insert(highscoresText.begin(), 10, "1.     0");
	updateHighscoresTexts();

	file.close();
}

void HighScores::saveToFile()
{
	file.open(filePath, std::ios_base::out | std::ios_base::trunc);

	for (size_t i = 0; i < highscores.size(); i++)
	{
		file << highscores.at(i);
		file << " ";
	}

	file.close();
}

int HighScores::insertNewScore(int score)
{
	// if score is big enough to be on highscore
	if (score > highscores.at(9))
	{
		struct {
			bool operator()(const int& a,const int& b) const { return a > b; }
		} static customLess;

		// overwrite worst score, and sort highscores
		highscores.at(9) = score;
		std::stable_sort(highscores.begin(), highscores.end(), customLess);
		updateHighscoresTexts();
	}
}

void HighScores::updateHighscoresTexts()
{
	for (size_t i = 0; i < highscoresText.size(); i++)
	{
		highscoresText.at(i) = (std::to_string(i+1) + ".    " + std::to_string(highscores.at(i)));
	}
}

const std::string& HighScores::getHighscoresText(int position)
{
	return highscoresText.at(position);
}