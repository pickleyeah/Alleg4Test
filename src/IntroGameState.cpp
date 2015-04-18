#include "IntroGameState.h"

#include <allegro.h>

IntroGameState::~IntroGameState(void)
{
	delete area;
}

void IntroGameState::Init()
{
	area = Area::CreateTestArea();
}
void IntroGameState::Shutdown()
{
}

void IntroGameState::Pause()
{
}
void IntroGameState::Resume()
{
}

void IntroGameState::ProcessInput()
{
	area->ProcessInput();
}

void IntroGameState::Update(Game *game, double dt)
{
	area->Update(dt);
}

void IntroGameState::Render(Game *game, BITMAP *buffer)
{
	clear_to_color(buffer, makecol(0,0,0));

	// Draw grid
	Vec2 offset = Vec2(0,0);
	area->Render(buffer, offset);
}
