#pragma once
#include "../../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <string>

/*
 * ============================================================================
 * DirectionalAnimationComponent - Animations avec directions
 * ============================================================================
 * Gère les animations directionnelles pour les spritesheets RPG
 * (Walk, Attack, Jump avec 4 directions + animations spéciales)
 * ============================================================================
 */

enum class Direction
{
    Down = 0,
    Up = 1,
    Left = 2,
    Right = 3
};

class DirectionalAnimationComponent : public ECS::Component
{

public:
    std::string currentAnimation;
    Direction currentDirection;

    Uint32 lastTimeFrame;
    int animationSpeed;
    bool isPlaying = true;

        int currentFrame;
    int tileSize;

    DirectionalAnimationComponent()
        : currentAnimation("walk"),
          currentDirection(Direction::Down),
          currentFrame(0),
          lastTimeFrame(0),
          animationSpeed(150),
          tileSize(16) {}

    DirectionalAnimationComponent(const std::string &anim, Direction dir, int speed = 150, int size = 16)
        : currentAnimation(anim),
          currentDirection(dir),
          currentFrame(0),
          lastTimeFrame(0),
          animationSpeed(speed),
          tileSize(size) {}
    void pause()
    {
        isPlaying = false;
    }

    void play()
    {
        if (!isPlaying)
        {
            isPlaying = true;
            lastTimeFrame = SDL_GetTicks(); // Reset le timer
        }
    }
    void stop()
    {
        isPlaying = false;
        currentFrame = 0;
    }

    void setAnimation(const std::string &anim)
    {
        if (currentAnimation != anim)
        {
            currentAnimation = anim;
            currentFrame = 0;
            lastTimeFrame = SDL_GetTicks();
        }
    }

    void setDirection(Direction dir)
    {
        if (currentDirection != dir)
        {
            currentDirection = dir;
            currentFrame = 0;
            lastTimeFrame = SDL_GetTicks();
        }
    }

    int getFrameCount() const
    {
        if (currentAnimation == "Walk")
        {
            return 4;
        }
        else
        {
            return 1;
        }
    }

    int getStartColumn() const
    {
        if (currentAnimation == "Walk" || currentAnimation == "Attack" || currentAnimation == "Jump")
        {
            return static_cast<int>(currentDirection); // Ligne = direction
        }
        else if (currentAnimation == "Dead")
        {
            return 0;
        }
        else if (currentAnimation == "Item")
        {
            return 1;
        }
        else if (currentAnimation == "Special1")
        {
            return 2;
        }
        else if (currentAnimation == "Special2")
        {
            return 3;
        }
        return 0;
    }
    int getRow() const
    {
        if (currentAnimation == "Walk")
        {
            return 0;
        }
        else if (currentAnimation == "Attack")
        {
            return 4;
        }
        else if (currentAnimation == "Jump")
        {
            return 5;
        }
        else
        {
            return 6;
        }
    }

    void init() override
    {
        lastTimeFrame = SDL_GetTicks();
        std::cout << "[DirectionalAnimationComponent] Initialized with animation: "
                  << currentAnimation << "\n";
    }
};