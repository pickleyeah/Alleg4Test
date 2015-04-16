#include "Vec2.h"
#include <math.h>

Vec2::Vec2(void)
{
	x = 0;
	y = 0;
}

Vec2::Vec2(float nx, float ny)
{
	x = nx;
	y = ny;
}


Vec2::~Vec2(void)
{
}

float Vec2::Length()
{
	return (float)sqrt(x*x + y*y);
}

void Vec2::Normalise()
{
	float l = Length();
	x /= l;
	y /= l;
}
