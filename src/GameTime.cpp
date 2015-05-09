#include "GameTime.h"

double GameTime::m_frameTime = 0;
double GameTime::m_totalElapsedTime = 0;

void GameTime::Update(double dt)
{
	m_frameTime = dt;
	m_totalElapsedTime += dt;
}