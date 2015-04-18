#pragma once

#include <allegro.h>
#include <vector>

class GameState;

class Game
{
public:
	static const int SCREEN_X = 640;
	static const int SCREEN_Y = 480;

	~Game();

	bool Init(const char* title);
	void Shutdown();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void ProcessInput(double dt);
	void Update(double dt);
	void Render();

	// Getters/setters
	bool Finished() const { return m_finished; }

private:
	BITMAP *m_buffer;
	std::vector<GameState*> m_states;
	bool m_finished;
	const char *m_title;
};
