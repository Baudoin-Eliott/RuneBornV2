#include "Game.h"
#include <iostream>

// Systèmes de base de l'ECS
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/AnimationSystem.h"
#include "src/Components/PlayerComponent.h"

Game::Game() {
    m_isRunning = true;
    m_window = nullptr;
    m_renderer = nullptr;
}

Game::~Game() {
    // SDL gère les nullptr automatiquement
}

int Game::Init(const char* title, int x, int y, int width, int height, bool fullscreen) {
    
    // Initialisation SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Création de la fenêtre
    Uint32 windowFlags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    m_window = SDL_CreateWindow(title, x, y, width, height, windowFlags);
    
    if (!m_window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Création du renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!m_renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    m_isRunning = true;

    // Initialiser les systèmes ECS
    setupSystems();

    // Créer une entité de test
    createTestEntity();

    std::cout << "[Game] Initialized successfully!\n";
    return 0;
}

void Game::setupSystems() {
    // Système d'animation
    auto* animSys = m_manager.addSystem<AnimationSystem>(32, 32);
    animSys->setPriority(30);

    // Système de rendu (toujours en dernier)
    auto* renderSys = m_manager.addSystem<RenderSystem>(m_renderer);
    renderSys->setPriority(100);

    std::cout << "[Game] Systems initialized\n";
}

void Game::createTestEntity() {
    ECS::Entity& player = m_manager.createEntity("Player");
    player.addLayer(LAYER_PLAYER);
    
    // Composants de base
    player.addComponent<TransformComponent>(400, 300, 2.0f);
    player.addComponent<SpriteComponent>(32, 32);
    
    // NOUVEAU : Composant spécifique au joueur
    player.addComponent<PlayerComponent>(250.0f);  // Vitesse de 250
    
    auto& sprite = player.getComponent<SpriteComponent>();
    sprite.renderLayer = LAYER_PLAYER;

    // Tester le level up
    auto& playerComp = player.getComponent<PlayerComponent>();
    playerComp.gainExperience(100);  // Devrait level up !

    std::cout << "[Game] Player created with level: " << playerComp.level << "\n";
}

void Game::HandleEvents() {
    while (SDL_PollEvent(&m_event)) {
        switch (m_event.type) {
            case SDL_QUIT:
                m_isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE) {
                    m_isRunning = false;
                }
                break;

            default:
                break;
        }
    }
}

void Game::Update(float deltaTime) {
    // Mise à jour de tous les systèmes
    m_manager.update(deltaTime);

    // Nettoyage des entités mortes
    m_manager.refresh();
}

void Game::Render() {
    // Clear de l'écran
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);

    // Le RenderSystem dessine automatiquement

    // Afficher
    SDL_RenderPresent(m_renderer);
}

void Game::Clean() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    
    std::cout << "[Game] Cleaned up\n";
}