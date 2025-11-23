#pragma once
#include "../Game.h"
#include "../../ECS/Utils/Menu.h"
#include "../Managers/AudioManager.h"
#include "../../ECS/Utils/GameState.h"
#include "../../ECS/Utils/UIManager.h"
#include <vector>
#include <string>

class MainMenu : public Menu
{

private:
    SDL_Renderer *renderer;
    Game *game;

    std::vector<std::string> choices = {"Play", "Options", "Credits", "Quit"};

    int selected = 0;

public:
    MainMenu(SDL_Renderer *rend, Game *g) : Menu("MainMenu"), renderer(rend), game(g) {}

    void onEnter() override
    {
        Menu::onEnter();
        if (!AudioManager::getInstance().hasLoadSound("MenuAccept"))
            AudioManager::getInstance().loadSound("Sounds/MenuMenuAccept", {"Sounds/Menu/Accept.wav", "Sounds/Menu/Accept2.wav", "Sounds/Menu/Accept3.wav", "Sounds/Menu/Accept4.wav", "Sounds/Menu/Accept5.wav", "Sounds/Menu/Accept6.wav", "Sounds/Menu/Accept7.wav", "Sounds/Menu/Accept8.wav", "Sounds/Menu/Accept9.wav"});
        if (!AudioManager::getInstance().hasLoadSound("MenuCancel"))
            AudioManager::getInstance().loadSound("MenuCancel", {"Sounds/Menu/Cancel.wav", "Sounds/Menu/Cancel2.wav"});
        if (!AudioManager::getInstance().hasLoadSound("Menu"))
            AudioManager::getInstance().loadSound("Menu", {"Sounds/Menu/Menu5.wav", "Sounds/Menu/Menu6.wav", "Sounds/Menu/Menu8.wav", "Sounds/Menu/Menu10.wav", "Sounds/Menu/Menu11.wav", "Sounds/Menu/Menu12.wav"});
        if (!AudioManager::getInstance().hasLoadSound("MenuMove"))
            AudioManager::getInstance().loadSound("MenuMove", {"Sounds/Menu/Move1.wav", "Sounds/Menu/Move2.wav", "Sounds/Menu/Move3.wav", "Sounds/Menu/Move4.wav", "Sounds/Menu/Move5.wav", "Sounds/Menu/Move6.wav", "Sounds/Menu/Move7.wav", "Sounds/Menu/Move8.wav"});
    }

    void onExit() override {}

    void update(float deltaTime) override
    {
        (void)deltaTime;
    }

    void handleInput(SDL_Event &event) override
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {

            case (SDLK_UP):
            case (SDLK_z):
                AudioManager::getInstance().playSound("MenuMove");
                selected--;
                if (selected < 0)
                    selected = choices.size() - 1;
                break;
            case (SDLK_DOWN):
            case (SDLK_s):
                AudioManager::getInstance().playSound("MenuMove");
                selected++;
                if (selected > choices.size() - 1)
                    selected = 0;
                break;
            case (SDLK_RETURN):
            case (SDLK_SPACE):
                AudioManager::getInstance().playSound("MenuAccept");
                executeOption();
                break;
            case (SDLK_ESCAPE):
                AudioManager::getInstance().playSound("MenuCancel");
                //game->SetState(game->previousState);
                break;
            }
        }
    }

    void render(SDL_Renderer *rend) override
    {

        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 180);
        SDL_Rect overlay = {0, 0, 800, 600}; // Adapter à ta résolution
        SDL_RenderFillRect(rend, &overlay);

        // 2. Titre "RUNEBORN" centré en haut
        // Pour l'instant, juste un rectangle (tu ajouteras du texte après)
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_Rect titleRect = {300, 100, 200, 50};
        SDL_RenderDrawRect(rend, &titleRect);

        // 3. Options du menu
        int startY = 300;
        int spacing = 50;

        for (size_t i = 0; i < choices.size(); ++i)
        {
            int y = startY + (i * spacing);

            // À TOI : Dessine un rectangle pour chaque option
            // Si i == selectedIndex → couleur différente (highlight)
            // Sinon → couleur normale

            if (i == selected)
            {
                SDL_SetRenderDrawColor(rend, 255, 255, 0, 255); // Jaune pour sélection
            }
            else
            {
                SDL_SetRenderDrawColor(rend, 100, 100, 100, 255); // Gris pour non-sélectionné
            }

            SDL_Rect optionRect = {300, y, 200, 40};
            SDL_RenderFillRect(rend, &optionRect);
        }
    }

private:

    void executeOption(){

        if (choices[selected] == "Play"){
            game->SetState(GameState::Playing);
            UIManager::getInstance().clearMenu();
        }else if (choices[selected] == "Options") {

            std::cout << "[MainMenu] Options not implemented yet\n";
            
        } else if (choices[selected] == "Credits") {

            std::cout << "[MainMenu] Credits not implemented yet\n";
            
        } else if (choices[selected] == "Quit") {

            SDL_Event quit_event;
            quit_event.type = SDL_QUIT;
            SDL_PushEvent(&quit_event);
        }

    }

};