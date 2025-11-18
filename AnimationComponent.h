#pragma once
#include "ECS.h"
#include "SDL3/SDL.h"
#include <map>
#include <string>


struct Animation {
    int index;      // La ligne (y) ou le décalage de l'animation sur le spritesheet
    int frames;     // Nombre total de frames dans cette animation
    int speed;      // Vitesse de l'animation (en ms par frame)
};

class AnimationComponent : public Component {
public:
    // État actuel de l'animation (ex: "Idle", "Walk", "Attack")
    std::string currentAnimState;

    // Frame actuellement affichée dans la boucle
    int currentFrame = 0;

    // Temps du dernier changement de frame (utilisé par le Système)
    int lastFrameTime = 0;

    // Collection de toutes les animations pour cette entité
    std::map<std::string, Animation> animations;

    AnimationComponent(const std::string& defaultState, std::map<std::string, Animation> anims)
        : currentAnimState(defaultState), animations(std::move(anims)) {}

    void init() override {}
    void update() override {}

    // Fonction pour changer l'animation (appelée par un StateSystem/InputSystem)
    void Play(const std::string& animName) {
        if (currentAnimState != animName) {
            currentAnimState = animName;
            currentFrame = 0; // Réinitialiser le frame quand on change d'animation
            lastFrameTime = SDL_GetTicks(); // Réinitialiser le temps
        }
    }
};