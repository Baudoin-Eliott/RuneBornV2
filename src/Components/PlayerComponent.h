#pragma once
#include "../../ECS/ECS.h"

/*
 * ============================================================================
 * PlayerComponent - Données du joueur de RuneBorn
 * ============================================================================
 * Ce composant contient les données SPÉCIFIQUES au joueur
 * 
 * Ne mettez ICI que ce qui est UNIQUE au joueur, pas des données génériques
 * comme la position (TransformComponent) ou la santé (HealthComponent)
 * ============================================================================
 */

class PlayerComponent : public ECS::Component {
public:
    // Vitesse de déplacement (pixels par seconde)
    float moveSpeed = 200.0f;
    
    // Vitesse de dash/esquive
    float dashSpeed = 400.0f;
    bool canDash = true;
    float dashCooldown = 1.0f;  // En secondes
    float dashTimer = 0.0f;
    
    // Statistiques
    int level = 1;
    int experience = 0;
    int experienceToNextLevel = 100;
    
    // ====================================================================
    // CONSTRUCTEURS
    // ====================================================================
    
    // Constructeur par défaut
    PlayerComponent() = default;
    
    // Constructeur avec vitesse custom
    PlayerComponent(float speed) : moveSpeed(speed) {}
    
    // ====================================================================
    // MÉTHODES UTILITAIRES
    // ====================================================================
    
    // Gagner de l'expérience
    void gainExperience(int amount) {
        experience += amount;
        
        // Level up ?
        while (experience >= experienceToNextLevel) {
            levelUp();
        }
    }
    
    // Monter de niveau
    void levelUp() {
        level++;
        experience -= experienceToNextLevel;
        experienceToNextLevel = static_cast<int>(experienceToNextLevel * 1.5f);
        
        // Bonus de level up
        // (vous pourrez ajouter des stats ici plus tard)
        
        std::cout << "[Player] Level UP! Now level " << level << "\n";
    }
    
    // Hook d'initialisation (appelé quand on ajoute le composant)
    void init() override {
        std::cout << "[PlayerComponent] Player initialized with speed: " 
                  << moveSpeed << "\n";
    }
};