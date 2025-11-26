#pragma once
#include "../Game.h"
#include "../../ECS/Utils/Menu.h"
#include "../Managers/AudioManager.h"
#include "../Managers/UIThemeManager.h"
#include "../../ECS/Utils/GameState.h"
#include "../../ECS/Utils/UIManager.h"
#include <vector>
#include <string>

class CreditsMenu : public Menu
{
private:
    SDL_Renderer *renderer;
    Game *game;
    std::vector<std::string> titles = {"Graphics", "Ceci est un titre", "Faut que ce sois long...", "Alors je vais mettre bcp de texte", "Ca suffie toujours pas", "Et quand y'en a plus...", "Ca finit jamais...", "HIP HIP HIP"};
    std::vector<std::string> texts = {"Thanks to : Pixel-boy and AAA", "Ceci est un texte", "Donc je vais ecrire", "Vraiment, Beaucoup ?", "J'ecris encore", "eh beh y'en as encore", "Nan jamais", "HOURA"};
    int selected = 0;
    int posY = 0;
    int minY = 0;
    std::vector<SDL_Rect> buttonRects;

    int scrollbarX = 750;
    int scrollbarY = 120;
    int scrollbarWidth = 10;
    int scrollbarHeight = 450;
    int thumbHeight = 50;

    bool isDraggingScrollbar = false;

    int getThumbPosition() const;

public:

    CreditsMenu(SDL_Renderer *rend, Game *g) : Menu("CreditsMenu"), renderer(rend), game(g) { minY = 390 - titles.size() * 100 + 80; }
    void onEnter() override;
    bool handleInput(SDL_Event &event) override;
    void render(SDL_Renderer *rend) override;
};