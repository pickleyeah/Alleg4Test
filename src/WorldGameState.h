#pragma once
#include "gamestate.h"
#include "Area.h"

enum TRANSITION_STATE
{
	STATE_NORMAL,
	STATE_FADEOUT,
	STATE_FADEIN,
};

class WorldGameState :
	public GameState
{
public:
	WorldGameState(void);
	~WorldGameState(void);

	static const int BLOCK_SIZE = 64;

	void Init();
	void Shutdown();

	void Pause();
	void Resume();

	void ProcessInput(double dt);
	void Update(Game *game, double dt);
	void Render(Game *game, ALLEGRO_BITMAP *buffer);

	void TransitionToArea(WARP_DETAILS_T *details);

private:
	static const double FADE_PERIOD;
	void DoAreaChange();

	bool m_paused;

	TRANSITION_STATE m_state;
	double m_transitionTime;

	Area* m_area;
	Area* m_newArea;
};

