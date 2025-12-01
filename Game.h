#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../ECS/Utils/GameState.h"

// Nouveau ECS
#include <ECS.h>
#include "ECS/Utils/Vector2D.h"
#include "ECS/Utils/TiledParser.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/AnimationComponent.h"
#include "ECS/Components/CameraComponent.h"

// Managers
#include "src/Managers/AudioManager.h"

// Layers du jeu
enum GameLayer
{
    LAYER_BACKGROUND = 0,
    LAYER_GROUND = 1,
    LAYER_ITEMS = 2,
    LAYER_MONSTERS = 3,
    LAYER_PLAYER = 4,
    LAYER_SPELLS = 5,
    LAYER_PARTICLES = 6,
    LAYER_UI = 7
};

class Game
{
private:
    bool m_isRunning;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Event m_event;
    ECS::Manager m_manager;
    float m_deltaTime;

    ECS::Entity *currentMap = nullptr;
    ECS::Entity *m_player = nullptr;
    CameraComponent *camera = nullptr;

public:
    GameState currentState = GameState::MainMenu;
    GameState previousState = GameState::MainMenu;
    
    Game();
    ~Game();

    int Init(const char *title, int x, int y, int width, int height, bool fullscreen);
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    void Clean();

    void SetState(GameState newState);
    GameState getState() const
    {
        return currentState;
    }

    GameState getPreviousState() const
    {
        return previousState;
    }

    SDL_Renderer *getRenderer() { return m_renderer; }

    bool isRunning() { return m_isRunning; }

    void loadMap(const std::string &mapPath, const std::string &spawnPointName);

private:
    void setupSystems();
    void createTestEntity();
    SDL_Texture *loadTexture(const char *filepath);
};