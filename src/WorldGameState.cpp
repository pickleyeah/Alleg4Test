#include "WorldGameState.h"

#include <allegro.h>

WorldGameState::~WorldGameState(void)
{
	delete area;
}

void WorldGameState::Init()
{
	area = Area::CreateTestArea();
}
void WorldGameState::Shutdown()
{
}

void WorldGameState::Pause()
{
}
void WorldGameState::Resume()
{
}

void WorldGameState::ProcessInput(double dt)
{
	area->ProcessInput(dt);
}

void WorldGameState::Update(Game *game, double dt)
{
	area->Update(dt);
}

void WorldGameState::Render(Game *game, BITMAP *buffer)
{
	clear_to_color(buffer, makecol(0,0,0));

	// Draw grid
	Vec2 offset = Vec2(0,0);
	area->Render(buffer, offset);
}
