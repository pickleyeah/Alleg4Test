#include "Game.h"

#include "GameState.h"
#include "InputManager.h"

Game::~Game()
{
	release_bitmap(m_buffer);
}

bool Game::Init(const char* title)
{
	m_finished = false;
	m_title = title;
	if (allegro_init() != 0)
		return false;
	set_window_title(m_title);
	
	install_keyboard();

	set_color_depth(32);
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, Game::SCREEN_X, Game::SCREEN_Y, 0, 0)) {
		allegro_message("Video Error: %s.\n", allegro_error);
		return false;
	}

	m_buffer = create_bitmap(Game::SCREEN_X, Game::SCREEN_Y);
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
}

void Game::ChangeState(GameState* state)
{
	if (!m_states.empty())
	{
		m_states.back()->Shutdown();
		m_states.pop_back();
	}
	m_states.push_back(state);
	m_states.back()->Init();
}

void Game::PushState(GameState* state)
{
	if (!m_states.empty())
		m_states.back()->Pause();
	m_states.push_back(state);
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

void Game::ProcessInput()
{
	Input::Update();
	// Process input
	if (Input::KeyDown(KEY_ESC))
		m_finished = true;

	m_states.back()->ProcessInput();
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
	m_states.back()->Draw(this, m_buffer);

	vsync();
	acquire_screen();
	blit(m_buffer, screen, 0,0, 0,0, SCREEN_X, SCREEN_Y);
	release_screen();
}

