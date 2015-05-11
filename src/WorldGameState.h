#pragma once
#include "gamestate.h"
#include <allegro5/allegro_font.h>
#include "Area.h"
#include "InputComponents.h"

enum WORLDSTATE
{
	WORLDSTATE_NORMAL,
	WORLDSTATE_FADEOUT,
	WORLDSTATE_SWAPAREAS,
	WORLDSTATE_FADEIN,
	WORLDSTATE_NPCTEXT_WRITING,
	WORLDSTATE_NPCTEXT_WAITFORPLAYER,
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

	void TriggerAreaTransition(WARP_DETAILS_T *details);
	void TriggerNPCTextDisplay(NPCText *text);

private:
	static const double FADE_PERIOD;
	void DoAreaChange();
	bool m_paused;

	WORLDSTATE m_state;
	double m_transitionTime;

	Area* m_area;
	Area* m_newArea;

	NPCText* m_npcText;
	ALLEGRO_FONT *m_font;
};

