#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <time.h>
#include "Game.h"
#include "WorldGameState.h"

static const int UPDATES_PER_SEC = 60;
static const double UPDATE_INTERVAL = 1.0 / (double)UPDATES_PER_SEC;

int main(int argc, char* argv[]) {
	clock_t prevTime, currTime;
	Game game;

	if (!game.Init("Nomekop"))
	{
		al_show_native_message_box(al_get_current_display(), "Error", NULL, "Cannot initialize game", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	game.PushState(new WorldGameState());
	prevTime = clock();
	double elapsedTime = 0;
	while (!game.Finished())
	{
		currTime = clock();
		double dt = ((double)(currTime - prevTime) / CLOCKS_PER_SEC);
		prevTime = clock();
		elapsedTime += dt;

		while (elapsedTime >= UPDATE_INTERVAL)
		{
			// Decouple game update and render functionality
			game.ProcessInput(UPDATE_INTERVAL);
			game.Update(UPDATE_INTERVAL);
			elapsedTime -= UPDATE_INTERVAL;
		}

		game.Render();
	}

	game.Shutdown();
	return 0;
}
