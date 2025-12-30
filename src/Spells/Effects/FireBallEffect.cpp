
#include "FireBallEffect.h"
#include "../../../ECS/Components/TransformComponent.h"
#include "../../../ECS/Components/SpriteComponent.h"
#include "../../../ECS/Components/CollisionComponent.h"
#include "../../../ECS/Components/AnimationComponent.h"
#include "../../Components/DirectionalAnimationComponent.h"
#include <SDL2/SDL.h>
#include "../../Managers/AssetsManager.h"

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

    auto &directionalComp = casterEntity->getComponent<DirectionalAnimationComponent>();

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

        auto &casterTransformComp = casterEntity->getComponent<TransformComponent>();

        spellEntity->addComponent<TransformComponent>(
            casterTransformComp.position.x + casterEntity->hasComponent<SpriteComponent>()
                ? casterEntity->getComponent<SpriteComponent>().width * casterTransformComp.scale / 2
                : 0,
            casterTransformComp.position.y);

        auto &projectileTransform = spellEntity->getComponent<TransformComponent>();
        projectileTransform.velocity = velocity;
        projectileTransform.rotation = velocity.AngleDegrees() - 90.0f;

        std::cout << "[FireBall] Created at pos: (" << projectileTransform.position.x << ", " << projectileTransform.position.y
                  << ") with velocity: (" << velocity.x << ", " << velocity.y << ")\n";
        
        SDL_Texture *texture = AssetsManager::getInstance().getTexture(
            SDL_GetRenderer(SDL_GetWindowFromID(1)),
            "Spells/FireBall.png");

        if (!texture) {
            std::cerr << "[FireBall] ERROR: Failed to load texture!\n";
            return;
        }

        std::cout << "[FireBall] Texture loaded successfully: " << texture << "\n";
        spellEntity->addComponent<SpriteComponent>(texture, 8, 12);
        auto &spriteComp = spellEntity->getComponent<SpriteComponent>();
        spriteComp.renderLayer = 5;


        std::map<std::string, Animation> animations = {
            {"Fly", Animation(0, 7, 100, true)}
        };

        spellEntity->addComponent<AnimationComponent>("Fly", animations);

        auto &animComp = spellEntity->getComponent<AnimationComponent>();
        animComp.Play("Fly");

        spellEntity->addComponent<CollisionComponent>(0, 0, 8, 12, "Spell");

    }
}

void FireBallEffect::OnUpdate(ECS::Entity *spellEntity, float deltaTime)
{
    lifetime -= deltaTime;
    if (lifetime <= 0)
    {
        OnExpire(spellEntity);
        spellEntity->destroy();
    }











}

void FireBallEffect::OnHit(ECS::Entity *targetEntity)
{
}

void FireBallEffect::OnExpire(ECS::Entity *spellEntity)
{
    std::cout << "[FireBall] Expired\n";
}
