#pragma once
#include <SDL2/SDL.h>

// Nouveau ECS
#include "ECS/ECS.h"
#include "ECS/Utils/Vector2D.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/AnimationComponent.h"

// Layers du jeu
enum GameLayer {
    LAYER_BACKGROUND = 0,
    LAYER_GROUND = 1,
    LAYER_ITEMS = 2,
    LAYER_MONSTERS = 3,
    LAYER_PLAYER = 4,
    LAYER_SPELLS = 5,
    LAYER_PARTICLES = 6,
    LAYER_UI = 7
};

class Game {
private:
    bool m_isRunning;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Event m_event;
    ECS::Manager m_manager;

public:
    Game();
    ~Game();

    int Init(const char* title, int x, int y, int width, int height, bool fullscreen);
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    void Clean();

    bool isRunning() { return m_isRunning; }
    
private:
    void setupSystems();
    void createTestEntity();
};