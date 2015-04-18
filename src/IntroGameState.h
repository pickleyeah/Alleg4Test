#pragma once
#include "gamestate.h"
#include "Area.h"

class IntroGameState :
	public GameState
{
public:
	IntroGameState(void) { };
	~IntroGameState(void);

	static const int BLOCK_SIZE = 64;

	void Init();
	void Shutdown();

	void Pause();
	void Resume();

	void ProcessInput(double dt);
	void Update(Game *game, double dt);
	void Render(Game *game, BITMAP *buffer);

private:
	bool m_paused;

	Area* area;
};

