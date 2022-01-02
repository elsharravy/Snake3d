#include "Random.h"

#include <stdexcept>

int Random::randInt(int min, int max)
{
	return (rd() % (max-min)) + min;
}

int* Random::randArrayWithoutRepetitions(int min, int max, int n)
{
	if (n < (max - min))
	{
		throw std::invalid_argument("n is less than max-min");
	}

	int* arr = new int[n];
	int* alreadyPicked = new int[n];

	for (size_t i = 0; i < n; i++)
	{
		alreadyPicked[i] = 0;
	}

	int randomNumber;

	for (size_t i = 0; i < n; i++)
	{

		while (true)
		{
			randomNumber = randInt(min, max);

			if (alreadyPicked[randomNumber] != 1)
			{
				arr[i] = randomNumber;
				alreadyPicked[randomNumber] = 1;
				break;
			}
		}
	}

	delete[] alreadyPicked;
	return arr;
}

std::random_device Random::rd;