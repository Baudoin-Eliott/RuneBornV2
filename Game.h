#pragma once
#include <SDL3/SDL.h>
#include "Components.h"


class Game
{
private:

	bool m_isRunning;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Event m_event;
	Manager m_manager;

public:
	Game();
	~Game();

	int Init(const char* title, int x, int y, int weigth, int height, bool fullscreen);
	void HandleEvents();
	void Update();


	bool isRunning() { return m_isRunning; };
	void Render();
	void Clean();

};

