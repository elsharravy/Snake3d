#ifndef COLOR_H
#define COLOR_H

struct Color
{
	float R;
	float G;
	float B;
	float A;

	Color(float R, float G, float B, float A);

	static Color Red();
};

#endif