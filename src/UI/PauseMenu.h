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
    PauseMenu(SDL_Renderer *rend, Game *g) : Menu("PauseMenu"), renderer(rend), game(g)
    {
        for (size_t i = 0; i < choices.size(); ++i)
        {
            int y = 300 + (i * 50);
            SDL_Rect buttonRect = {250, y - 5, 300, 50};
            buttonRects.push_back(buttonRect);
        }
    }
    void onEnter() override
    {
        Menu::onEnter();
        if (!AudioManager::getInstance().hasLoadSound("MenuAccept"))
            AudioManager::getInstance().loadSound("MenuAccept", {"Sounds/Menu/Accept.wav", "Sounds/Menu/Accept2.wav", "Sounds/Menu/Accept3.wav", "Sounds/Menu/Accept4.wav", "Sounds/Menu/Accept5.wav", "Sounds/Menu/Accept6.wav", "Sounds/Menu/Accept7.wav", "Sounds/Menu/Accept8.wav", "Sounds/Menu/Accept9.wav"});
        if (!AudioManager::getInstance().hasLoadSound("MenuCancel"))
            AudioManager::getInstance().loadSound("MenuCancel", {"Sounds/Menu/Cancel.wav", "Sounds/Menu/Cancel2.wav"});
        if (!AudioManager::getInstance().hasLoadSound("Menu"))
            AudioManager::getInstance().loadSound("Menu", {"Sounds/Menu/Menu5.wav", "Sounds/Menu/Menu6.wav", "Sounds/Menu/Menu8.wav", "Sounds/Menu/Menu10.wav", "Sounds/Menu/Menu11.wav", "Sounds/Menu/Menu12.wav"});
        if (!AudioManager::getInstance().hasLoadSound("MenuMove"))
            AudioManager::getInstance().loadSound("MenuMove", {"Sounds/Menu/Move1.wav", "Sounds/Menu/Move2.wav", "Sounds/Menu/Move3.wav", "Sounds/Menu/Move4.wav", "Sounds/Menu/Move5.wav", "Sounds/Menu/Move6.wav", "Sounds/Menu/Move7.wav", "Sounds/Menu/Move8.wav"});
    }

    void onExit() override {}

    bool handleInput(SDL_Event &event) override
    {
        if (event.type == SDL_MOUSEWHEEL)
        {
            if (event.wheel.y > 0)
            {
                AudioManager::getInstance().playSound("MenuMove");
                selected--;
                if (selected < 0)
                    selected = choices.size() - 1;
            }
            else if (event.wheel.y < 0)
            {
                AudioManager::getInstance().playSound("MenuMove");
                selected++;
                if (selected >= choices.size() - 1)
                    selected = 0;
            }
        }
        if (event.type == SDL_MOUSEMOTION)
        {
            for (size_t i = 0; i < buttonRects.size(); i++)
            {

                if (isMouseInRect(event.motion.x, event.motion.y, buttonRects[i]))
                {
                    if (i != selected)
                        AudioManager::getInstance().playSound("MenuMove");
                    selected = i;
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if (isMouseInRect(event.button.x, event.button.y, buttonRects[selected]))
                {
                    AudioManager::getInstance().playSound("MenuAccept");
                    executeOption();
                    return true;
                }
            }
        }
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
                return true;
                break;
            case (SDLK_DOWN):
            case (SDLK_s):
                AudioManager::getInstance().playSound("MenuMove");
                selected++;
                if (selected > choices.size() - 1)
                    selected = 0;
                return true;
                break;
            case (SDLK_RETURN):
            case (SDLK_SPACE):
                AudioManager::getInstance().playSound("MenuAccept");
                executeOption();
                return true;
                break;
            case (SDLK_ESCAPE):
                AudioManager::getInstance().playSound("MenuCancel");
                game->SetState(game->previousState);
                UIManager::getInstance().popMenu();
                return true;
                break;
            default:
                return false;
            }
        }
        return false;
    }

    void render(SDL_Renderer *rend) override
    {

        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, 800, 600};
        SDL_RenderFillRect(rend, &overlay);

        UIThemeManager::getInstance().renderPanel(250, 80, 300, 80);

        UIThemeManager::getInstance().renderTextCentered("PAUSED", 400, 120, UIThemeManager::getInstance().getCurrentColor());

        int startY = 300;
        int spacing = 50;

        for (size_t i = 0; i < choices.size(); ++i)
        {
            int y = startY + (i * spacing);

            SDL_Color textColor;
            if (i == static_cast<size_t>(selected))
            {
                UIThemeManager::getInstance().renderPanel(250, y - 5, 300, 50);
                textColor = UIThemeManager::getInstance().getCurrentSelectedColor();
            }
            else
            {
                textColor = UIThemeManager::getInstance().getCurrentColor();;
            }

            UIThemeManager::getInstance().renderTextCentered(choices[i], 400, y + 20, textColor);
        }
    }

private:
    void executeOption()
    {

        if (choices[selected] == "Resume")
        {
            game->SetState(GameState::Playing);
            UIManager::getInstance().clearMenu();
        }
        else if (choices[selected] == "Options")
        {

            UIManager::getInstance().pushMenu(new OptionsMenu(renderer, game));
        }
        else if (choices[selected] == "Main Menu")
        {

            UIManager::getInstance().clearMenu();
            UIManager::getInstance().pushMenu(new MainMenu(renderer, game));
            game->SetState(GameState::MainMenu);
        }
        else if (choices[selected] == "Quit")
        {

            SDL_Event quit_event;
            quit_event.type = SDL_QUIT;
            SDL_PushEvent(&quit_event);
        }
    }
};