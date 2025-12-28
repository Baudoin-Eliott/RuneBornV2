
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
    // Sauvegarder le caster pour utilisation ultérieure
    caster = casterEntity;

    // Vérifier que le caster a les composants nécessaires
    if (!casterEntity->hasComponent<DirectionalAnimationComponent>())
        return;

    auto& directionalComp = casterEntity->getComponent<DirectionalAnimationComponent>();

    // ERREUR CORRIGÉE: Calculer la vélocité selon la direction
    // Les méthodes Up(), Down(), etc. retournent un nouveau Vector2D, elles ne modifient pas l'objet
    Vector2D velocity;
    switch (directionalComp.currentDirection)
    {
    case Direction::Up:
        velocity = Vector2D::Up() * speed;    // CORRIGÉ: Ajout de break et assignation correcte
        break;
    case Direction::Down:
        velocity = Vector2D::Down() * speed;  // CORRIGÉ
        break;
    case Direction::Left:
        velocity = Vector2D::Left() * speed;  // CORRIGÉ
        break;
    case Direction::Right:
        velocity = Vector2D::Right() * speed; // CORRIGÉ
        break;
    }

    // ERREUR CORRIGÉE: Déclarer la variable dans un scope accessible
    if (casterEntity->hasComponent<TransformComponent>())
    {
        // La variable est maintenant dans le bon scope avec des accolades
        auto& casterTransformComp = casterEntity->getComponent<TransformComponent>();

        // Ajouter le TransformComponent au projectile avec la position du caster et la vélocité
        spellEntity->addComponent<TransformComponent>(
            casterTransformComp.position.x,
            casterTransformComp.position.y
        );

        // Définir la vélocité du projectile
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
