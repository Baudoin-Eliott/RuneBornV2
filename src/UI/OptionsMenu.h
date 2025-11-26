#pragma once
#include "../Game.h"
#include "../../ECS/Utils/Menu.h"
#include "../Managers/AudioManager.h"
#include "../../ECS/Utils/GameState.h"
#include "../../ECS/Utils/UIManager.h"
#include "../Managers/UIThemeManager.h"
#include "MainMenu.h"
#include "CreditsMenu.h"
#include <vector>
#include <string>
#include "../../ECS/Utils/Slider.h"

class OptionsMenu : public Menu
{

private:
    SDL_Renderer *renderer;
    Game *game;

    std::vector<std::string> choices = {"Bamboo", "Bocal", "Bubble", "Dark", "Map", "Metal", "Metal2", "Metal3", "Red", "Wood", "Wood2", "Wood3", "Wood4"};
    int selected = 0;
    int selectedThemeIndex = 0;
    std::map<std::string, SDL_Rect> buttonRects;

    int posY;

    Slider musicSlider;
    Slider sfxSlider;
    Slider themeSlider;

    SDL_Rect keysButtonRect;

public:
    OptionsMenu(SDL_Renderer *rend, Game *g);

    void CalculeButtonsRects();
    void onEnter() override;
    bool handleInput(SDL_Event &event) override;
    void render(SDL_Renderer *rend) override;
};