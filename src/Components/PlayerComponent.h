#pragma once
#include <ECS.h>
#include "../Spells/SpellRecognizer.h"
#include <optional>


class PlayerComponent : public ECS::Component {
public:

    float moveSpeed = 200.0f;

    std::optional<RecognitionResult> currentSpell;
    

    PlayerComponent() = default;
    
    PlayerComponent(float speed) : moveSpeed(speed) {}
    
    void fuzeSpell(RecognitionResult otherSpell);

    void init() override {
    }
};