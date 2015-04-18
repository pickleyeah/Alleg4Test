#include <allegro.h>
#include <time.h>
#include "Game.h"
#include "IntroGameState.h"

static const int UPDATES_PER_SEC = 60;
static const double UPDATE_INTERVAL = 1.0 / (double)UPDATES_PER_SEC;

int main(int argc, char* argv[]) {
	clock_t prevTime, currTime;
	Game game;

	if (!game.Init("Game"))
	{
		allegro_message("Cannot initalize game.\n");
		return 1;
	}

	game.PushState(new IntroGameState());
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
			game.ProcessInput();
			game.Update(UPDATE_INTERVAL);
			elapsedTime -= UPDATE_INTERVAL;
		}

		game.Render();
	}

	game.Shutdown();
	return 0;
}
END_OF_MAIN();
