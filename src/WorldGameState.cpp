#include "WorldGameState.h"
#include "Sprite.h"
#include "XMLAreaLoader.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>

const double WorldGameState::FADE_PERIOD = 0.5f;

WorldGameState::WorldGameState(void) :
	m_area(nullptr),
	m_newArea(nullptr),
	m_npcText(nullptr),
	m_font(nullptr),
	m_transitionTime(0),
	m_state(WORLDSTATE_NORMAL),
	m_startupLevel(nullptr)
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

	const char *filename = m_startupLevel ? m_startupLevel : "Data/Areas/Area_1.xml";
	m_area = XMLAreaLoader::LoadAreaFromXMLFile(filename, this);
	if (!m_area)
	{
		al_show_native_message_box(al_get_current_display(), "Error", std::string("Could not open area: ").append(filename).c_str(), NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
	m_area->SetPlayer(Entity::CreateEntity("Player"));
	m_area->Init();

	m_font = al_create_builtin_font();
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
	switch (m_state)
	{
	case WORLDSTATE_NORMAL:
		m_area->ProcessInput(dt);
		break;
	case WORLDSTATE_SWAPAREAS:
		// An area change was triggered, swap the map and start the fadein
		m_state = WORLDSTATE_FADEIN;
		m_transitionTime = FADE_PERIOD;
		DoAreaChange();
		break;
	case WORLDSTATE_NPCTEXT_WRITING:
		m_state = WORLDSTATE_NPCTEXT_WAITFORPLAYER;
		break;
	case WORLDSTATE_NPCTEXT_WAITFORPLAYER:
		if (Input::KeyPressed(ALLEGRO_KEY_SPACE))
		{
			if (++m_npcTextIndex >= m_npcText->Strings.size())
				m_state = WORLDSTATE_NORMAL;
			else
				m_state = WORLDSTATE_NPCTEXT_WRITING;
		}
		break;
	}
}

void WorldGameState::Update(Game *game, double dt)
{
	switch (m_state)
	{
	case WORLDSTATE_NORMAL:
		m_area->Update(dt);
		break;
	case WORLDSTATE_FADEOUT:
	case WORLDSTATE_FADEIN:
		m_transitionTime -= dt;
		if (m_transitionTime <= 0)
			m_state = m_state == WORLDSTATE_FADEOUT ? WORLDSTATE_SWAPAREAS : WORLDSTATE_NORMAL;
		break;
	}
}

void WorldGameState::Render(Game *game, ALLEGRO_BITMAP *buffer)
{
	// Clear the buffer bitmap
	al_set_target_bitmap(buffer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	// Draw scene
	Vec2 offset = Vec2(0, 0);
	m_area->Render(offset);

	// Draw NPC text if applicable
	if (m_state == WORLDSTATE_NPCTEXT_WRITING || m_state == WORLDSTATE_NPCTEXT_WAITFORPLAYER)
	{
		Vec2 topLeft(0, al_get_bitmap_height(buffer) / 3 * 2);
		Vec2 bottomRight(al_get_bitmap_width(buffer), al_get_bitmap_height(buffer));
		topLeft = topLeft + Vec2(32, 32);
		bottomRight = bottomRight - Vec2(32, 32);
		al_draw_filled_rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, al_map_rgba_f(1, 1, 1, 1));
		Vec2 textPos = topLeft + Vec2(32, 32);
		al_draw_textf(m_font, al_map_rgb(0, 0, 0), textPos.x, textPos.y, 0, m_npcText->Strings[m_npcTextIndex].c_str());
	}

	// Draw fade if applicable
	if (m_state == WORLDSTATE_FADEOUT || m_state == WORLDSTATE_SWAPAREAS)
	{
		float alpha = 1 - (m_transitionTime * 2);
		al_draw_filled_rectangle(0, 0, al_get_bitmap_width(buffer), al_get_bitmap_height(buffer), al_map_rgba_f(0, 0, 0, alpha));
	}
	else if (m_state == WORLDSTATE_FADEIN)
	{
		float alpha = (m_transitionTime * 2);
		al_draw_filled_rectangle(0, 0, al_get_bitmap_width(buffer), al_get_bitmap_height(buffer), al_map_rgba_f(0, 0, 0, alpha));
	}

	return;
}

void WorldGameState::TriggerAreaTransition(WARPDETAILS_T *details)
{
	m_newArea = XMLAreaLoader::LoadAreaFromXMLFile(details->area.c_str(), this);
	if (!m_newArea)
	{
		std::string message = "Couldn't load map: " + details->area;
		al_show_native_message_box(al_get_current_display(), "Error", message.c_str(), NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	m_newArea->SetStartPosAndDir(details->pos, details->dir);
	
	m_state = WORLDSTATE_FADEOUT;
	m_transitionTime = FADE_PERIOD;
}

void WorldGameState::TriggerNPCTextDisplay(NPCText *text)
{
	m_npcText = text;
	m_npcTextIndex = 0;
	m_state = WORLDSTATE_NPCTEXT_WRITING;
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
