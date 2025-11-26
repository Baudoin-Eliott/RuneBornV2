#pragma once
#include "../../ECS/Utils/Menu.h"
#include "../Game.h"

class RunesMenu : public Menu
{
    SDL_Renderer *renderer;
    Game *game;
    int level;

    RunesMenu(SDL_Renderer* rend, Game *g) : Menu("RunesMenu"), renderer(rend), game(g) {}

    void onEnter() override
    {
        Menu::onEnter();
    }

    void render(SDL_Renderer *rend) override
    {
    }
};