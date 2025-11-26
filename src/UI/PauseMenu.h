#pragma once
#include "../Game.h"
#include "../../ECS/Utils/Menu.h"
#include "../Managers/AudioManager.h"
#include "../../ECS/Utils/GameState.h"
#include "../../ECS/Utils/UIManager.h"
#include "MainMenu.h"
#include "CreditsMenu.h"
#include "OptionsMenu.h"
#include <vector>
#include <string>

class PauseMenu : public Menu
{

private:
    SDL_Renderer *renderer;
    Game *game;

    std::vector<std::string> choices = {"Resume", "Options", "Main Menu", "Quit"};
    int selected = 0;
    std::vector<SDL_Rect> buttonRects;

public:
    PauseMenu(SDL_Renderer *rend, Game *g);
    void onEnter() override;

    bool handleInput(SDL_Event &event) override;
    void render(SDL_Renderer *rend) override;

private:
    void executeOption();
    
};