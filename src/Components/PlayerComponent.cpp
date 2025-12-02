#include "PlayerComponent.h"

void PlayerComponent::fuzeSpell(RecognitionResult otherSpell)
{

    if (!currentSpell){
        currentSpell = otherSpell;
        return;
    }
}