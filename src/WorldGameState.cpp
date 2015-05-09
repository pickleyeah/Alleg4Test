#include "WorldGameState.h"
#include "Sprite.h"

const double WorldGameState::FADE_PERIOD = 0.5f;

WorldGameState::WorldGameState(void) :
	m_area(nullptr),
	m_newArea(nullptr),
	m_transitionTime(0),
	m_state(STATE_NORMAL)
{

}

WorldGameState::~WorldGameState(void)
{
	delete m_area;
}

void WorldGameState::Init()
{
	// Preload sprites
	Sprite::PreloadSpriteList("Data/Sprites.psl");

	//m_area = Area::CreateTestArea(nullptr, this);
	m_area = Area::LoadArea("Data/Areas/Area1.lvl", nullptr, this);
	//m_area->Write("Data/Areas/Area2.lvl");
	m_area->SetStartPosAndDir(Vec2(1, 1), DIR_SOUTH);
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
	//if (Input::KeyPressed(KEY_1)) // Transition areas
	//	TransitionToArea(Area::CreateTestArea(m_area->GetPlayer(), this));
	//else if (Input::KeyPressed(KEY_2)) // Transition areas
	//	TransitionToArea(Area::CreateTestArea2(m_area->GetPlayer(), this));

	// 
	switch (m_state)
	{
	case STATE_NORMAL:
		m_area->ProcessInput(dt);
		break;
	case STATE_FADEIN:
		// An area change was triggered
		DoAreaChange();
		break;
	}
	if (m_state != STATE_NORMAL)
		return;
}

void WorldGameState::Update(Game *game, double dt)
{
	if (m_state != STATE_NORMAL)
	{
		m_transitionTime -= dt;
		if (m_transitionTime <= 0)
		{
			m_state = m_state == STATE_FADEOUT ? STATE_FADEIN : STATE_NORMAL;
			if (m_state == STATE_FADEIN)
				m_transitionTime = FADE_PERIOD;
		}
		return;
	}
	m_area->Update(dt);
}

void WorldGameState::Render(Game *game, ALLEGRO_BITMAP *buffer)
{
	// Clear the buffer bitmap
	al_set_target_bitmap(buffer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_set_target_backbuffer(al_get_current_display());

	// Draw grid
	Vec2 offset = Vec2(0,0);
	m_area->Render(buffer, offset);
	// TODO: find way to fade to/from black
	if (m_state == STATE_FADEOUT)
	{

	}
	else if (m_state == STATE_FADEIN)
	{

	}
	
}

void WorldGameState::TransitionToArea(WARP_DETAILS_T *details)
{
	/*if (strcmp(details->areaName, "1") == 0)
		m_newArea = Area::CreateTestArea(m_area->GetPlayer(), this);
	else if (strcmp(details->areaName, "2") == 0)
		m_newArea = Area::CreateTestArea2(m_area->GetPlayer(), this);
	else*/
		m_newArea = Area::LoadArea(details->areaName, m_area->GetPlayer(), this);
	m_newArea->SetStartPosAndDir(details->startPos, details->startDir);
	
	m_state = STATE_FADEOUT;
	m_transitionTime = FADE_PERIOD;
}

void WorldGameState::DoAreaChange()
{
	if (m_newArea != nullptr)
	{
		m_newArea->SetPlayer(m_area->GetPlayer());
		m_newArea->Init();
		delete m_area;
		m_area = m_newArea;
		m_newArea = nullptr;
	}
}
