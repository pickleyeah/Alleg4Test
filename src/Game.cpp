#include "Game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "GameState.h"
#include "InputManager.h"
#include "Sprite.h"
#include "GameTime.h"

Game::~Game()
{
}

bool Game::Init(const char* title)
{
	m_finished = false;
	m_title = title;
	if (!al_init())
		return false;
	
	al_install_keyboard();

	m_display = al_create_display(Game::SCREEN_X, Game::SCREEN_Y);
	al_set_window_title(m_display, m_title);

	al_init_image_addon();
	al_init_primitives_addon();

	m_buffer = al_create_bitmap(Game::SCREEN_X, Game::SCREEN_Y);
	Input::Init();

	return true;
}

void Game::Shutdown()
{
	while (!m_states.empty())
	{
		m_states.back()->Shutdown();
		m_states.pop_back();
	}
	Sprite::ReleaseAllSprites();
	al_destroy_bitmap(m_buffer);
	al_destroy_display(m_display);
}

void Game::ChangeState(GameState* state)
{
	if (!m_states.empty())
	{
		m_states.back()->Shutdown();
		m_states.pop_back();
	}
	m_states.push_back(std::unique_ptr<GameState>(state));
	m_states.back()->Init();
}

void Game::PushState(GameState* state)
{
	if (!m_states.empty())
		m_states.back()->Pause();
	m_states.push_back(std::unique_ptr<GameState>(state));
	m_states.back()->Init();
}

void Game::PopState()
{
	if (!m_states.empty())
	{
		m_states.back()->Shutdown();
		m_states.pop_back();
	}
	if (!m_states.empty())
		m_states.back()->Resume();
}

void Game::ProcessInput(double dt)
{
	Input::Update();
	GameTime::Update(dt);
	// Process input
	if (Input::KeyDown(ALLEGRO_KEY_ESCAPE))
		m_finished = true;

	m_states.back()->ProcessInput(dt);
}

void Game::Update(double dt)
{
	m_states.back()->Update(this, dt);
}

void Game::Render()
{
	m_states.back()->Render(this, m_buffer);

	al_set_target_backbuffer(al_get_current_display());
	al_draw_bitmap(m_buffer, 0, 0, 0);
	al_wait_for_vsync();
	al_flip_display();
}

