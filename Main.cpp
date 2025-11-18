#pragma once
#include "Game.h"

int main() {

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	int frameStart;
	int frameTime;


	Game* game = new Game();
	game->Init("RuneBorn V2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->isRunning()) {
		frameStart = SDL_GetTicks();
		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	game->Clean();
	return 0;
}