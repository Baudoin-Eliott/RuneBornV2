#pragma once

#include "../../ECS/ECS.h"
#include "../../ECS/Components/TransformComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/DirectionalAnimationComponent.h"
#include <SDL2/SDL.h>


class PlayerInputSystem : public ECS::System
{
public:
    PlayerInputSystem();

    void update(float deltaTime) override;
};