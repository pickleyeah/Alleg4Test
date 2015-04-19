#include "WorldGameState.h"

#include <allegro.h>

WorldGameState::WorldGameState(void) :
	m_area(nullptr),
	m_newArea(nullptr)
{

}

WorldGameState::~WorldGameState(void)
{
	delete m_area;
}

void WorldGameState::Init()
{
	m_area = Area::CreateTestArea(nullptr, this);
	m_area->Init();
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
		TransitionToArea(Area::CreateTestArea(m_area->GetPlayer(), this));
	else if (Input::KeyPressed(KEY_2)) // Transition areas
		TransitionToArea(Area::CreateTestArea2(m_area->GetPlayer(), this));

	m_area->ProcessInput(dt);

	// An area change was triggered
	if (m_newArea != nullptr)
	{
		m_newArea->SetPlayer(m_area->GetPlayer());
		m_newArea->Init();
		delete m_area;
		m_area = m_newArea;
		m_newArea = nullptr;
	}
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
	m_newArea = area;
}
