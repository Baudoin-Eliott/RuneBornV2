#pragma once

#include "../../ECS/ECS.h"
#include "../../ECS/Components/TransformComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/DirectionalAnimationComponent.h"
#include <SDL2/SDL.h>

/*
 * ============================================================================
 * PlayerInputSystem - Gère les contrôles du joueur
 * ============================================================================
 * Ce système lit les inputs clavier et modifie la vélocité du joueur
 *
 * Contrôles :
 * - ZQSD : Déplacement
 * - ESPACE : Dash (plus tard)
 * ============================================================================
 */

class PlayerInputSystem : public ECS::System
{
public:
    PlayerInputSystem()
    {
        requireComponent<TransformComponent>();
        requireComponent<PlayerComponent>();
    }

    void init() override
    {
        std::cout << "[PlayerInputSystem] Initialized\n";
    }

    void update(float deltaTime) override
    {
        (void)deltaTime; // Pas utilisé ici

        const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

        for (auto entity : getEntities())
        {
            auto &transform = entity->getComponent<TransformComponent>();
            auto &player = entity->getComponent<PlayerComponent>();

            transform.velocity.x = 0;
            transform.velocity.y = 0;

            bool isMoving = false;


            Direction newDirection = Direction::Down;


            if (keyState[SDL_SCANCODE_Z] || keyState[SDL_SCANCODE_W])
            {
                transform.velocity.y = -1;
                newDirection = Direction::Up;
                isMoving = true;
            }
            if (keyState[SDL_SCANCODE_S])
            {
                transform.velocity.y = 1;
                newDirection = Direction::Down;
                isMoving = true;
            }
            if (keyState[SDL_SCANCODE_Q] || keyState[SDL_SCANCODE_A])
            {
                transform.velocity.x = -1;
                newDirection = Direction::Left;
                isMoving = true;
            }
            if (keyState[SDL_SCANCODE_D])
            {
                transform.velocity.x = 1;
                newDirection = Direction::Right;
                isMoving = true;
            }

            if (transform.velocity.x != 0 && transform.velocity.y != 0)
            {
                transform.velocity.Normalize();
            }

            transform.velocity.x *= player.moveSpeed;
            transform.velocity.y *= player.moveSpeed;

            if (entity->hasComponent<DirectionalAnimationComponent>())
            {
                auto &anim = entity->getComponent<DirectionalAnimationComponent>();
                if (isMoving)
                {
                    anim.setAnimation("Walk");
                    anim.setDirection(newDirection);
                    anim.play();
                }
                else
                {
                    anim.stop();
                    
                }
            }
        }
    }
};