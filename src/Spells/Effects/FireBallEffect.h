#pragma once
#include "../SpellEffect.h"
#include "../SpellPattern.h"
#include <ECS.h>

class FireBallEffect : public SpellEffect {

private:
int power;
int damage;
int speed;
float lifetime;
float creationTime;

ECS::Entity* caster;

public:
    FireBallEffect(const SpellPattern& pattern, int pow);

    // ERREUR CORRIGÉE: La signature doit correspondre à SpellEffect.h (2 paramètres au lieu d'1)
    // Setup initial du projectile
    void OnCast(ECS::Entity* spellEntity, ECS::Entity* casterEntity) override;

    // Update chaque frame (mouvement, collision, etc.)
    void OnUpdate(ECS::Entity* spellEntity, float deltaTime) override;

    // Quand ça touche une cible
    void OnHit(ECS::Entity* targetEntity) override;

    // Quand le sort expire
    void OnExpire(ECS::Entity* spellEntity) override;
};