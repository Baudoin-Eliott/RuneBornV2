
#include "FireBallEffect.h"
#include "../../../ECS/Components/TransformComponent.h"
#include "../../../ECS/Components/SpriteComponent.h"
#include "../../../ECS/Components/CollisionComponent.h"
#include <SDL2/SDL.h>

FireBallEffect::FireBallEffect(const SpellPattern& pattern, int pow)
    : power(pow)
    , damage(pattern.baseDamage * (pow / 100.0f))
    , speed(pattern.speed)  
    , lifetime(pattern.lifetime)
    , caster(nullptr)
{
    std::cout << "[FireBall] Created with power " << pow << "% (damage: " << damage << ")\n";
}

void FireBallEffect::OnCast(ECS::Entity *spellEntity)
{
}

    void FireBallEffect::OnUpdate(ECS::Entity *spellEntity, float deltaTime)
{
}

void FireBallEffect::OnHit(ECS::Entity *targetEntity)
{
}

void FireBallEffect::OnExpire(ECS::Entity *spellEntity)
{
}
