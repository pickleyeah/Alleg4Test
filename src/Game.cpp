#include "Game.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#include "GameState.h"
#include "InputManager.h"
#include "Sprite.h"

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

	//al_set_color_depth(32);
	/*if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, Game::SCREEN_X, Game::SCREEN_Y, 0, 0)) {
		al_show_native_message_box(al_get_current_display(), "Error", NULL, "Video Error: %s, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		allegro_message("Video Error: %s.\n", allegro_error);
		return false;
	}*/

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
	// Process input
	if (Input::KeyDown(ALLEGRO_KEY_ESCAPE))
		m_finished = true;

	m_states.back()->ProcessInput(dt);
}

void Game::Update(double dt)
{
	// Update game state
	// Prevent dt from jumping ahead too much
	if (dt > 0.1)
		dt = 0.1;
	m_states.back()->Update(this, dt);
}

void Game::Render()
{
	m_states.back()->Render(this, m_buffer);

	al_wait_for_vsync();
	al_draw_bitmap(m_buffer, 0, 0, 0);
	al_flip_display();
	//vsync();
	//acquire_screen();
	//blit(m_buffer, screen, 0,0, 0,0, SCREEN_X, SCREEN_Y);
	//release_screen();
}

