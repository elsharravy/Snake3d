#include "Color.h"

Color::Color(float R, float G, float B, float A) : R(R), G(G), B(B), A(A)
{

}

Color Color::Red()
{
	return Color(1.0f, 0.0f, 0.0f, 0.0f);
}