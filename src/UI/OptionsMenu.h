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
    OptionsMenu(SDL_Renderer *rend, Game *g)
        : Menu("OptionsMenu"),
          renderer(rend),
          game(g),
          musicSlider(375, 150, 350, 10, 0, 100, 75, true),
          sfxSlider(375, 355, 350, 10, 0, 100, 100, true),
          themeSlider(60, 250, 10, 250, -320, 80, 0, false, true, -10),
          keysButtonRect({400, 480, 250, 60})
    {
        musicSlider.onValueChanged = [](int val)
        {
            AudioManager::getInstance().setMusicVolume(val / 100.0f);
        };

        sfxSlider.onValueChanged = [](int val)
        {
            AudioManager::getInstance().setSoundVolume(val / 100.0f);
        };

        themeSlider.onValueChanged = [this](int val)
        {
            posY = val;
            CalculeButtonsRects();
        };

        std::string currentTheme = UIThemeManager::getInstance().getCurrentTheme();
        currentTheme = "Theme" + currentTheme;
        for (size_t i = 0; i < choices.size(); i++)
        {
            if (currentTheme == choices[i])
            {
                selectedThemeIndex = i;
                break;
            }
        }
        CalculeButtonsRects();
    }

    void CalculeButtonsRects()
    {
        buttonRects.clear();
        int Y = 180 + posY;
        int spacing = 60;
        for (size_t i = 0; i < choices.size(); i++)
        {
            if (Y > 250 && Y < 480)
                buttonRects[choices[i]] = {80, Y - 5, 120, 40};
            Y += 50;
        }
    }

    void
    onEnter() override
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

    bool handleInput(SDL_Event &event) override
    {
        if (musicSlider.handleInput(event))
            return true;
        if (sfxSlider.handleInput(event))
            return true;
        if (themeSlider.handleInput(event))
            return true;

        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {

            for (auto &pair : buttonRects)
            {
                if (isMouseInRect(event.button.x, event.button.y, pair.second))
                {
                    AudioManager::getInstance().playSound("MenuAccept");
                    UIThemeManager::getInstance().setTheme("Theme" + pair.first);
                }
            }

            if (isMouseInRect(event.button.x, event.button.y, keysButtonRect))
            {
                AudioManager::getInstance().playSound("MenuAccept");
                std::cout << "[OptionsMenu] Keys button clicked!\n";
                return true;
            }
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            if (isMouseInRect(event.motion.x, event.motion.y, keysButtonRect))
            {
                if (selected != 0)
                    AudioManager::getInstance().playSound("MenuMove");
                selected = 0;
            }
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case (SDLK_ESCAPE):
                AudioManager::getInstance().playSound("MenuCancel");
                UIManager::getInstance().popMenu();
                return true;
            }
        }

        return false;
    }

    void render(SDL_Renderer *rend) override
    {

        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(rend, 60, 60, 60, 255);
        SDL_Rect overlay = {0, 0, 800, 600};
        SDL_RenderFillRect(rend, &overlay);

        UIThemeManager::getInstance().renderPanel(10, 30, 175, 50);
        UIThemeManager::getInstance().renderText("OPTIONS", 35, 40, {255, 255, 255, 255});

        UIThemeManager::getInstance().renderPanel(400, 70, 250, 60);
        UIThemeManager::getInstance().renderText("Music Volume", 425, 75);
        musicSlider.render();

        UIThemeManager::getInstance().renderPanel(400, 275, 250, 60);
        UIThemeManager::getInstance().renderText("SFX Volume", 425, 280);
        sfxSlider.render();

        UIThemeManager::getInstance().renderPanel(keysButtonRect.x, keysButtonRect.y, keysButtonRect.w, keysButtonRect.h);

        SDL_Color keysTextColor = (selected == 0) ? UIThemeManager::getInstance().getCurrentSelectedColor() : UIThemeManager::getInstance().getCurrentColor();
        ;
        UIThemeManager::getInstance().renderTextCentered("Keys", 525, keysButtonRect.y + 25, keysTextColor);

        UIThemeManager::getInstance().renderPanel(30, 130, 175, 50);
        UIThemeManager::getInstance().renderText("Theme :", 70, 135, {255, 255, 255, 255});

        UIThemeManager::getInstance().renderPanel(40, 235, 205, 275, "assets/Ui/Theme/Theme Wood/nine_path_panel.png");

        themeSlider.render();

        int scrollOffset = themeSlider.value;

        int Y = 180 + posY;
        int spacing = 60;

        for (size_t i = 0; i < choices.size(); i++)
        {
            if (Y > 250 && Y < 480)
            {
                UIThemeManager::getInstance().renderPanel(80, Y - 5, 120, 40);
                SDL_Color color;
                if (i == selectedThemeIndex)
                    color = UIThemeManager::getInstance().getCurrentSelectedColor();
                else
                    color = UIThemeManager::getInstance().getCurrentColor();
                UIThemeManager::getInstance().renderText(choices[i], 90, Y, color);
            }
            Y += 50;
        }
    }
};