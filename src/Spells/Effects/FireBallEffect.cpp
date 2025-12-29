
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
    caster = casterEntity;

    if (!casterEntity->hasComponent<DirectionalAnimationComponent>())
        return;

    auto& directionalComp = casterEntity->getComponent<DirectionalAnimationComponent>();

    Vector2D velocity;
    switch (directionalComp.currentDirection)
    {
    case Direction::Up:
        velocity = Vector2D::Up() * speed;    
        break;
    case Direction::Down:
        velocity = Vector2D::Down() * speed;  
        break;
    case Direction::Left:
        velocity = Vector2D::Left() * speed;
        break;
    case Direction::Right:
        velocity = Vector2D::Right() * speed;
        break;
    }

    if (casterEntity->hasComponent<TransformComponent>())
    {
        auto& casterTransformComp = casterEntity->getComponent<TransformComponent>();

        spellEntity->addComponent<TransformComponent>(
            casterTransformComp.position.x,
            casterTransformComp.position.y
        );

        auto& projectileTransform = spellEntity->getComponent<TransformComponent>();
        projectileTransform.velocity = velocity;

        // TODO: Ajouter un SpriteComponent pour afficher le projectile
        // TODO: Ajouter un CollisionComponent pour détecter les collisions
        // TODO: Sauvegarder le temps de création pour gérer le lifetime

        std::cout << "[FireBall] Cast from (" << casterTransformComp.position.x
                  << ", " << casterTransformComp.position.y
                  << ") with velocity (" << velocity.x << ", " << velocity.y << ")\n";
    }
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
