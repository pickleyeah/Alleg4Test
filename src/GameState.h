#pragma once

#include "Game.h"

class GameState
{
public:
	virtual ~GameState() { };

	virtual void Init();
	virtual void Shutdown() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;
	bool Paused() { return m_paused; }

	virtual void ProcessInput(double dt) = 0;
	virtual void Update(Game *game, double dt) = 0;
	virtual void Render(Game *game, BITMAP *buffer) = 0;

private:
	bool m_paused;
};
