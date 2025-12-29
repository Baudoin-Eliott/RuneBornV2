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

    void OnCast(ECS::Entity* spellEntity, ECS::Entity* casterEntity) override;

    void OnUpdate(ECS::Entity* spellEntity, float deltaTime) override;

    void OnHit(ECS::Entity* targetEntity) override;

    void OnExpire(ECS::Entity* spellEntity) override;
};