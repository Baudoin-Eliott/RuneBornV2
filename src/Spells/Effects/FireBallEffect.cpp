
#include "FireBallEffect.h"
#include "../../../ECS/Components/TransformComponent.h"
#include "../../../ECS/Components/SpriteComponent.h"
#include "../../../ECS/Components/CollisionComponent.h"
#include "../../Components/DirectionalAnimationComponent.h"
#include <SDL2/SDL.h>

FireBallEffect::FireBallEffect(const SpellPattern &pattern, int pow)
    : power(pow), damage(pattern.baseDamage * (pow / 100.0f)), speed(pattern.speed), lifetime(pattern.lifetime), caster(nullptr)
{
    std::cout << "[FireBall] Created with power " << pow << "% (damage: " << damage << ")\n";
}

void FireBallEffect::OnCast(ECS::Entity *spellEntity, ECS::Entity *casterEntity)
{
    if (!casterEntity->hasComponent<DirectionalAnimationComponent>())
        return;
    auto& directionalComp = casterEntity->getComponent<DirectionalAnimationComponent>();
    Vector2D velocity;
    switch (directionalComp.currentDirection)
    {
    case Direction::Up:
        velocity.Up();
    case Direction::Down:
        velocity.Down();
    case Direction::Left:
        velocity.Left();
    case Direction::Right:
        velocity.Right();
    }

    if (casterEntity->hasComponent<TransformComponent>())
        auto& casterTransformComp = casterEntity->getComponent<TransformComponent>();
    if (casterTransformComp)
    spellEntity->addComponent<TransformComponent>(casterTransformComp->position.x, casterTransformComp->position.y);

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
