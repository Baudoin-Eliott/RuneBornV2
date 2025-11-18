#include "Game.h"
#include <iostream>


Game::Game() {
	m_isRunning = true;
	m_window = nullptr;
}

Game::~Game()
{
	delete m_window;
}

int Game::Init(const char* title, int x, int y, int width, int height, bool fullscreen) {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
		std::cerr << "Eroor while initialized video & event..." << SDL_GetError() << std::endl;
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(title, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0, &m_window, &m_renderer) == 0) {
		std::cerr << "Eroor while created Window & Renderer..." << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	m_isRunning = true;	

	return 0;
}

void Game::HandleEvents()
{
	SDL_PollEvent(&m_event);
	switch (m_event.type) {

	case SDL_EVENT_QUIT:
		m_isRunning = false;
		break;
	default:
		break;


	}
}

void Game::Update() {

	//##TODO
}

void Game::Render()
{
}

void Game::Clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();



}
