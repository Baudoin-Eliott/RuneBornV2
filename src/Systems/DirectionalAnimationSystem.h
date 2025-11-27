#pragma once
#include <ECS.h>
#include "../../ECS/Components/SpriteComponent.h"
#include "../Components/DirectionalAnimationComponent.h"
#include <SDL2/SDL.h>
#include "../Managers/AudioManager.h"


class DirectionalAnimationSystem : public ECS::System
{
public:
    DirectionalAnimationSystem();

    void update(float deltaTime) override;
};