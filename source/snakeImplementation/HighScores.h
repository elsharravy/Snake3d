#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <vector>
#include <string>
#include <fstream>



class HighScores
{
	std::vector<int> highscores;
	std::vector<std::string> highscoresText;

	void updateHighscoresTexts();

	std::string filePath;

	std::fstream file;
public:
	HighScores(std::string filePath);

	void loadFromFile();
	void saveToFile();
	// returns position of new score, -1 if score is too weak 
	int insertNewScore( int score );

	const std::string& getHighscoresText(int position);
};

#endif