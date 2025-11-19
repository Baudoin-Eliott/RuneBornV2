#pragma once
#include "../../ECS/ECS.h"
#include "../../ECS/Components/SpriteComponent.h"
#include "../Components/DirectionalAnimationComponent.h"
#include <SDL2/SDL.h>

/*
 * ============================================================================
 * DirectionalAnimationSystem - Met à jour les animations directionnelles
 * ============================================================================
 * Ce système gère les animations avec directions pour les spritesheets RPG type characters
 * ============================================================================
 */

class DirectionalAnimationSystem : public ECS::System
{
public:
    DirectionalAnimationSystem()
    {
        requireComponent<SpriteComponent>();
        requireComponent<DirectionalAnimationComponent>();
    }

    void init() override
    {

        std::cout << "[DirectionalAnimationSystem] initialized\n";
    }

    void update(float deltaTime) override
    {
        (void)deltaTime;

        Uint32 currentTime = SDL_GetTicks();

        for (auto entity : getEntities())
        {
            auto &sprite = entity->getComponent<SpriteComponent>();
            auto &anim = entity->getComponent<DirectionalAnimationComponent>();

            if (!anim.isPlaying)
            {
                int col = anim.getStartColumn() + anim.currentFrame;
                int row = anim.getRow();

                sprite.srcRect.x = col * anim.tileSize;
                sprite.srcRect.y = row * anim.tileSize;
                sprite.srcRect.w = anim.tileSize;
                sprite.srcRect.h = anim.tileSize;

                continue;
            }
            Uint32 elapsed = currentTime - anim.lastTimeFrame;

            if (elapsed >= static_cast<Uint32>(anim.animationSpeed))
            {
                anim.currentFrame++;
                anim.lastTimeFrame = currentTime;
            }
            if (anim.currentFrame >= anim.getFrameCount())
            {
                anim.currentFrame = 0;
            }

            int col = anim.getStartColumn();
            int row = anim.getRow() + anim.currentFrame;

            sprite.srcRect.x = col * anim.tileSize;
            sprite.srcRect.y = row * anim.tileSize;
            sprite.srcRect.w = anim.tileSize;
            sprite.srcRect.h = anim.tileSize;
        }
    }
};