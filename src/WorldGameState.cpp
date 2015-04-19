#include "WorldGameState.h"

#include <allegro.h>

WorldGameState::~WorldGameState(void)
{
	delete m_area;
}

void WorldGameState::Init()
{
	m_area = Area::CreateTestArea(nullptr);
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
	if (Input::KeyPressed(KEY_1)) // Transition areas
		TransitionToArea(Area::CreateTestArea(m_area->GetPlayer()));
	else if (Input::KeyPressed(KEY_2)) // Transition areas
		TransitionToArea(Area::CreateTestArea2(m_area->GetPlayer()));
	m_area->ProcessInput(dt);
}

void WorldGameState::Update(Game *game, double dt)
{
	m_area->Update(dt);
}

void WorldGameState::Render(Game *game, BITMAP *buffer)
{
	clear_to_color(buffer, makecol(0,0,0));

	// Draw grid
	Vec2 offset = Vec2(0,0);
	m_area->Render(buffer, offset);
}

void WorldGameState::TransitionToArea(Area *area)
{
	delete m_area;
	m_area = area;
}
