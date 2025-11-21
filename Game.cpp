#include "Game.h"
#include <iostream>

// Systèmes de base de l'ECS
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/AnimationSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/TileMapRenderSystem.h"

// Systèmes perso
#include "src/Systems/PlayerInputSystem.h"
#include "src/Systems/DirectionalAnimationSystem.h"

// Component Perso
#include "src/Components/PlayerComponent.h"
#include "src/Components/DirectionalAnimationComponent.h"

Game::Game()
{
    m_isRunning = true;
    m_window = nullptr;
    m_renderer = nullptr;
}

Game::~Game()
{
    // SDL gère les nullptr automatiquement
}

int Game::Init(const char *title, int x, int y, int width, int height, bool fullscreen)
{

    // Initialisation SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cerr << "Error initializing SDL_image: " << IMG_GetError() << std::endl;
        return 1;
    }
    std::cout << "[Game] SDL_image initialized\n";

    // Création de la fenêtre
    Uint32 windowFlags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    m_window = SDL_CreateWindow(title, x, y, width, height, windowFlags);

    if (!m_window)
    {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Création du renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer)
    {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    m_isRunning = true;

    (void)ECS::getComponentTypeID<TransformComponent>(); // ID 0
    (void)ECS::getComponentTypeID<SpriteComponent>();    // ID 1
    (void)ECS::getComponentTypeID<AnimationComponent>(); // ID 2

    std::cout << "[Game] TransformComponent ID: " << ECS::getComponentTypeID<TransformComponent>() << "\n";
    std::cout << "[Game] SpriteComponent ID: " << ECS::getComponentTypeID<SpriteComponent>() << "\n";
    std::cout << "[Game] AnimationComponent ID: " << ECS::getComponentTypeID<AnimationComponent>() << "\n";

    
    // Initialiser les systèmes ECS
    setupSystems();



    // Créer une entité de test
    createTestEntity();

    std::cout << "[Game] Initialized successfully!\n";
    return 0;
}

void Game::setupSystems()
{

    // inputs
    auto *inputSys = m_manager.addSystem<PlayerInputSystem>();
    inputSys->setPriority(10);

    // mouvement
    auto *movementSys = m_manager.addSystem<MovementSystem>();
    movementSys->setPriority(20);

    // Système d'animation
    auto *dirAnimSys = m_manager.addSystem<DirectionalAnimationSystem>();
    dirAnimSys->setPriority(30);

    // camera
    auto *cameraSys = m_manager.addSystem<CameraSystem>();
    cameraSys->setPriority(40);

    // TilemapRender
    auto *tilemapRenderSys = m_manager.addSystem<TileMapRenderSystem>(m_renderer);
    tilemapRenderSys->setPriority(99);

    // Système de rendu (toujours en dernier)
    auto *renderSys = m_manager.addSystem<RenderSystem>(m_renderer);
    renderSys->setPriority(100);

    std::cout << "[Game] Systems initialized\n";
}

void Game::createTestEntity()
{
    ECS::Entity &player = m_manager.createEntity("Player");
    player.addLayer(LAYER_PLAYER);

    // Composants de base
    player.addComponent<TransformComponent>(400, 300, 1.0f);
    player.addComponent<SpriteComponent>(32, 32);
    player.addComponent<PlayerComponent>(100.0f);
    player.addComponent<DirectionalAnimationComponent>("Walk", Direction::Down, 150, 16);
    

    SDL_Texture *texture = loadTexture("assets/Actor/Characters/Boy/SpriteSheet.png");
    auto &sprite = player.getComponent<SpriteComponent>();
    sprite.setTexture(texture);
    sprite.renderLayer = LAYER_PLAYER;

    auto &transform = player.getComponent<TransformComponent>();
    transform.velocity = Vector2D(0, 0);

    ECS::Entity &map = m_manager.createEntity("Map");
    map.addComponent<TileMapComponent>();
    auto &tilemapCompo = map.getComponent<TileMapComponent>();
    TiledParser::loadFromFile("assets/Backgrounds/Maps/map1.tmx", tilemapCompo, m_renderer);

    ECS::Entity &camera = m_manager.createEntity("Camera");
    camera.addComponent<CameraComponent>(800, 600);
    auto &cameracomp = camera.getComponent<CameraComponent>();
    cameracomp.zoom = 2.f;
    cameracomp.setBounds(0, tilemapCompo.getMapWidthInPixels(), 0, tilemapCompo.getMapHeightInPixels());

    auto renderSys = m_manager.getSystem<RenderSystem>();
    renderSys->setCamera(&cameracomp);

    auto tilemapSys = m_manager.getSystem<TileMapRenderSystem>();
    tilemapSys->setCamera(&cameracomp);

    auto cameraSys = m_manager.getSystem<CameraSystem>();
    cameraSys->setTarget(&player);
}

SDL_Texture *Game::createColorTexture(int width, int height, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(
        0,          // flags (déprécié dans SDL2, mettre 0)
        width,      // largeur
        height,     // hauteur
        32,         // depth (bits par pixel)
        0xFF000000, // Rmask
        0x00FF0000, // Gmask
        0x0000FF00, // Bmask
        0x000000FF  // Amask
    );

    if (!surface)
    {
        std::cerr << "[Game] Failed to create surface :" << SDL_GetError() << "\n";
        return nullptr;
    }

    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, r, g, b));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    if (!texture)
    {
        std::cerr << "[game] Failed to create texture :" << SDL_GetError() << "\n";
        return nullptr;
    }

    SDL_FreeSurface(surface);

    return texture;
}

SDL_Texture *Game::loadTexture(const char *filepath)
{
    SDL_Surface *surface = IMG_Load(filepath);

    if (!surface)
    {
        std::cerr << "[Game] Failed to load image :" << filepath << "--" << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    if (!texture)
    {
        std::cerr << "[Game] failed to create Texture from Surface -" << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_FreeSurface(surface);
    std::cout << "[Game] Texture load from :" << filepath << std::endl;
    return texture;
}

void Game::HandleEvents()
{
    while (SDL_PollEvent(&m_event))
    {
        switch (m_event.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (m_event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
            break;

        default:
            break;
        }
    }
}

void Game::Update(float deltaTime)
{

    // Mise à jour de tous les systèmes
    m_manager.update(deltaTime);

    // Nettoyage des entités mortes
    m_manager.refresh();
}

void Game::Render()
{
    // Clear de l'écran
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);

    auto *tileMapSys = m_manager.getSystem<TileMapRenderSystem>();
    if (tileMapSys){
        tileMapSys->update(0);
    }


    auto *renderSys = m_manager.getSystem<RenderSystem>();
    if (renderSys)
    {
        renderSys->update(0); // Le deltaTime n'est pas utilisé par RenderSystem
    }

    // Afficher
    SDL_RenderPresent(m_renderer);
}

void Game::Clean()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    std::cout << "[Game] Cleaned up\n";
}