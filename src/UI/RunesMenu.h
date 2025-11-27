#pragma once
#include "../../ECS/Utils/Menu.h"
#include "../Game.h"

class RunesMenu : public Menu
{
    SDL_Renderer *renderer;
    Game *game;
    int level;
    int spacingX;
    int spacingY;
    bool isCasting;
    float tolerance;
    std::vector<int> connections;
    Vector2D lastMousePos;

public:
    RunesMenu(SDL_Renderer *rend, Game *g, int dif);
    void onEnter() override;

    bool handleInput(SDL_Event &event) override;
    void render(SDL_Renderer *rend) override;
    void onExit() override;

private:
    int calculateNearestPoint(Vector2D pos);
    Vector2D getPosById(int id);
    bool haveConnect(int point);
};