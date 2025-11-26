#pragma once
#include "../Game.h"
#include "../../ECS/Utils/Menu.h"
#include "../Managers/AudioManager.h"
#include "../Managers/UIThemeManager.h"
#include "../../ECS/Utils/GameState.h"
#include "../../ECS/Utils/UIManager.h"
#include "CreditsMenu.h"
#include "OptionsMenu.h"
#include <vector>
#include <string>

class MainMenu : public Menu
{

private:
    SDL_Renderer *renderer;
    Game *game;

    std::vector<std::string> choices = {"Play", "Options", "Credits", "Quit"};

    int selected = 0;

    std::vector<SDL_Rect> buttonRects;

public:
    MainMenu(SDL_Renderer *rend, Game *g);

    void onEnter() override;

    bool handleInput(SDL_Event &event) override;
    
    void render(SDL_Renderer *rend) override;
private:
    void executeOption();
};