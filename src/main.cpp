#include <allegro.h>
#include <time.h>
#include "Game.h"
#include "IntroGameState.h"

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
	while (!game.Finished())
	{
		currTime = clock();
		double dt = ((double)(currTime - prevTime) / CLOCKS_PER_SEC);
		prevTime = clock();

		game.Update(dt);
	}

	game.Shutdown();
	return 0;
}
END_OF_MAIN();
