#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random
{
	static std::random_device rd;

public:
	// generate random int in range [min,max)
	static int randInt(int min, int max);

	// recommend using for small ranges max-min
	static int* randArrayWithoutRepetitions(int min, int max, int n);
};

#endif