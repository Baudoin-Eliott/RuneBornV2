#pragma once
#include <ECS.h>
#include "../Spells/SpellRecognizer.h"
#include <optional>
class PlayerComponent : public ECS::Component
{

private:
    float moveSpeed = 200.0f;
    int hp = 100;
    int maxhp = 100;
    int mana = 20;
    int maxmana = 100;

    std::optional<RecognitionResult>
        currentSpell;

public:
    PlayerComponent() = default;

    PlayerComponent(float speed) : moveSpeed(speed) {}

    void fuzeSpell(RecognitionResult otherSpell);

    void init() override {}

    // getter

    float getSpeed() { return moveSpeed; }
    int getCurrentHp() { return hp; }
    int getMaxHp() { return maxhp; }

    int getCurrentMana() { return mana; }
    int getMaxMana() { return maxmana; }
    std::optional<RecognitionResult> getCurrentSpeel() { return currentSpell; }

    bool isAlive() { return hp <= 0; }

    // mod
    void damage(int dmg) { hp -= dmg; }
    void heal(int amout) { hp = std::min(hp + amout, maxhp); }

    void useMana(int amout) { mana -= amout; }

    void resetSpell() { currentSpell.reset(); }
};