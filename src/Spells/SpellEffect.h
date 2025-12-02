// SpellEffect.h
#pragma once
#include <ECS.h>
class SpellEffect {
public:
    virtual ~SpellEffect() = default;
    
    virtual void OnCast(ECS::Entity* spellEntity, ECS::Entity* casterEntity) {}
    
    virtual void OnUpdate(ECS::Entity* spellEntity, float deltaTime) {}
    
    virtual void OnHit(ECS::Entity* targetEntity) {}

    virtual void OnExpire(ECS::Entity* spellEntity) {}
};