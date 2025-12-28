#pragma once
#include <ECS.h>
#include <SDL2/SDL.h>
#include "../Managers/UIThemeManager.h"
#include "../Components/PlayerComponent.h"
#include "map"

class HUDSystem : public ECS::System
{
private:
    SDL_Renderer *renderer = nullptr;
    std::map<std::string, SDL_Texture *> spellIconTextures;

    const int BAR_X = 20;
    const int BAR_Y = 20;
    const int BAR_WIDTH = 200;
    const int BAR_HEIGHT = 20;
    const int BAR_SPACING = 30;

    // Valeurs pour l'exemple (à remplacer par les vraies stats du joueur)
    int currentHP = 80.0f;
    int maxHP = 100.0f;
    int currentMana = 50.0f;
    int maxMana = 100.0f;
    std::string currentSpellName;

public:
    HUDSystem()
    {
        // Le HUD ne dépend d'aucun composant spécifique
        // Il affiche juste l'UI en overlay
    }

    void setRenderer(SDL_Renderer *rend)
    {
        renderer = rend;
    }
    SDL_Texture *loadSpellIcon(std::string spellName)
    {
        if (spellIconTextures.find(spellName) != spellIconTextures.end())
        {
            return spellIconTextures[spellName];
        }

        // CORRIGÉ: Construire le chemin comme std::string d'abord
        std::string fullPath = "assets/Spells/Icons/" + spellName + ".png";
        SDL_Surface *surface = IMG_Load(fullPath.c_str());

        if (!surface)
        {
            std::cerr << "[HUDSystem] Error while creating surface from : " << fullPath << "\n ";
            std::cerr << "[HUDSystem] Error :" << SDL_GetError() << "\n";
            return nullptr;
        }
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
        if (!tex)
        {
            std::cerr << "[HUDSystem] Error while loading tetxure from surface ! \n";
            std::cerr << "[HUDSystem] Error :" << SDL_GetError() << "\n";
            return nullptr;
        }

        SDL_FreeSurface(surface);
        spellIconTextures[spellName] = tex;
        return tex;
    }

    // Dessine une barre avec bordure
    void drawBar(int x, int y, int width, int height, float current, float max, SDL_Color bgColor, SDL_Color fillColor)
    {
        if (!renderer)
            return;

        // Bordure noire
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect borderRect = {x - 2, y - 2, width + 4, height + 4};
        SDL_RenderDrawRect(renderer, &borderRect);

        // Fond
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_Rect bgRect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &bgRect);

        // Remplissage
        float fillRatio = (max > 0) ? (current / max) : 0.0f;
        fillRatio = std::max(0.0f, std::min(1.0f, fillRatio));

        SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        SDL_Rect fillRect = {x, y, static_cast<int>(width * fillRatio), height};
        SDL_RenderFillRect(renderer, &fillRect);
    }

    void render(SDL_Renderer *rend) override
    {
        if (!renderer)
            renderer = rend;

        // TODO: Récupérer les vraies valeurs du joueur
        // Pour l'instant, on utilise des valeurs de test

        // HP
        drawBar(
            BAR_X, BAR_Y,
            BAR_WIDTH, BAR_HEIGHT,
            currentHP, maxHP,
            {40, 40, 40, 255},
            {200, 50, 50, 255});

        UIThemeManager::getInstance().renderText(
            "HP: " + std::to_string(static_cast<int>(currentHP)) + "/" + std::to_string(static_cast<int>(maxHP)),
            BAR_X, BAR_Y - BAR_HEIGHT / 2,
            {255, 255, 255, 255});

        // Mana
        drawBar(
            BAR_X, BAR_Y + BAR_SPACING,
            BAR_WIDTH, BAR_HEIGHT,
            currentMana, maxMana,
            {40, 40, 40, 255},
            {50, 100, 200, 255});

        UIThemeManager::getInstance().renderText(
            "MP: " + std::to_string(static_cast<int>(currentMana)) + "/" + std::to_string(static_cast<int>(maxMana)),
            BAR_X, BAR_Y + BAR_SPACING - BAR_HEIGHT / 2,
            {255, 255, 255, 255});

        SDL_Texture *spellIcon = loadSpellIcon(currentSpellName);
        if (spellIcon)
        {
            SDL_Rect destRect = {18, 80, 48, 48};
            SDL_RenderCopy(renderer, spellIcon, nullptr, &destRect);
        }
    }

    void update(float deltaTime) override
    {

        for (auto &entity : entities)
        {
            if (entity->hasComponent<PlayerComponent>())
            {
                auto &player = entity->getComponent<PlayerComponent>();
                currentHP = player.getCurrentHp();
                currentMana = player.getCurrentMana();
                if (player.getCurrentSpeel().has_value())
                {
                    currentSpellName = player.getCurrentSpeel().value().spellName;
                    std::cout << currentSpellName << std::endl;
                }
                else
                    currentSpellName = "Empty";
            }
        }
    }
};
