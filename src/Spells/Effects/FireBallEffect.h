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
    
    // À TOI : Setup initial du projectile
    void OnCast(ECS::Entity* spellEntity) override;
    
    // À TOI : Update chaque frame (mouvement, collision, etc.)
    void OnUpdate(ECS::Entity* spellEntity, float deltaTime) override;
    
    // À TOI : Quand ça touche une cible
    void OnHit(ECS::Entity* targetEntity) override;
    
    // À TOI : Quand le sort expire
    void OnExpire(ECS::Entity* spellEntity) override;
};