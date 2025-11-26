#include "Game.h"
#include <iostream>

// Components de base de l'ECS (IMPORTANT: avant les systèmes!)
#include "ECS/Components/CollisionComponent.h"
#include "ECS/Components/TileMapComponent.h"

// Systèmes de base de l'ECS
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/AnimationSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/TileMapRenderSystem.h"
#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Systems/DebugRenderSystem.h"
#include "ECS/Systems/TriggerSystem.h"

// Systèmes perso
#include "src/Systems/PlayerInputSystem.h"
#include "src/Systems/DirectionalAnimationSystem.h"

// Component Perso
#include "src/Components/PlayerComponent.h"
#include "src/Components/DirectionalAnimationComponent.h"

// Managers
#include "src/Managers/AudioManager.h"
#include "ECS/Utils/UIManager.h"
#include "src/UI/MainMenu.h"
#include "src/UI/PauseMenu.h"
#include "src/UI/RunesMenu.h"

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

    if (!AudioManager::getInstance().init())
    {
        std::cerr << "[Game] Failed to initialize AudioManager\n";
        return 1;
    }
    std::cout << "[Game] AudioManager initialized\n";

    // Charger les footsteps (2 variations)
    if (!AudioManager::getInstance().loadSound("footstep", {"Sounds/Elemental/Grass.wav", "Sounds/Elemental/Grass2.wav"}))
    {
        std::cerr << "[Game] Failed to load footstep sounds\n";
    }

    // Charger le son de téléportation
    if (!AudioManager::getInstance().loadSound("teleport", {"Sounds/Magic & Skill/Fx.wav"}))
    {
        std::cerr << "[Game] Failed to load teleport sound\n";
    }

    std::cout << "[Game] All sounds loaded\n";

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

    AudioManager::getInstance().playMusic("adventure", 2000);
    std::cout << "[Game] Music started\n";

    UIManager::getInstance().setRenderer(m_renderer);

    UIManager::getInstance().pushMenu(new MainMenu(m_renderer, this));

    // Initialiser UIThemeManager
    if (!UIThemeManager::getInstance().init(m_renderer))
    {
        std::cerr << "[Game] Failed to initialize UIThemeManager\n";
        return 1;
    }
    std::cout << "[Game] UIThemeManager initialized\n";

    std::cout << "[Game] Initialized successfully!\n";
    return 0;
}

void Game::loadMap(const std::string &mapPath, const std::string &spawnPointName)
{
    std::cout << "[Game] Loading map: " << mapPath << "\n";
    std::cout << "[Game] Spawn point: " << spawnPointName << "\n";

    if (currentMap)
    {
        currentMap->destroy();
    }

    ECS::Entity &newMap = m_manager.createEntity("Map");
    newMap.addComponent<TileMapComponent>();
    auto &tileMapComp = newMap.getComponent<TileMapComponent>();

    std::string reelPath = "assets/Backgrounds/Maps/" + mapPath;
    if (!TiledParser::loadFromFile(reelPath, tileMapComp, m_renderer))
    {
        std::cerr << "[Game] ERROR : Failed to load map: " + reelPath + "\n";
        return;
    }
    currentMap = &newMap;

    auto collisionSys = m_manager.getSystem<CollisionSystem>();
    collisionSys->setTileMapEntity(currentMap);

    auto triggerSys = m_manager.getSystem<TriggerSystem>();
    triggerSys->setTileMapEntity(currentMap);

    auto debugRenderSys = m_manager.getSystem<DebugRenderSystem>();
    debugRenderSys->setTileMapEntity(currentMap);

    auto tilemapSystems = m_manager.getSystems<TileMapRenderSystem>();
    for (auto *sys : tilemapSystems)
        sys->setCamera(camera);

    TiledObject *spawn = tileMapComp.getObjectByName(spawnPointName);
    if (!spawn)
    {
        std::cerr << "[Game] no spawn point found named: " << spawnPointName << " on map: " << reelPath << "\n";
        return;
    }
    if (m_player)
    {
        auto &transform = m_player->getComponent<TransformComponent>();
        transform.position.x = spawn->x;
        transform.position.y = spawn->y;
        std::cout << "[Game] player teleported to (" << spawn->x << "," << spawn->y << ")\n";
    }

    if (camera)
    {
        auto tileMapComp = currentMap->getComponent<TileMapComponent>();
        float mapWidth = tileMapComp.getMapWidthInPixels();
        float mapHeight = tileMapComp.getMapHeightInPixels();

        // Calculer le zoom nécessaire pour remplir la fenêtre
        float zoomX = camera->viewportWidth / mapWidth;
        float zoomY = camera->viewportHeight / mapHeight;

        // Prendre le plus grand zoom (pour remplir complètement)
        float optimalZoom = std::max(zoomX, zoomY);

        // Option A : Forcer le zoom optimal
        // camera->zoom = optimalZoom;

        // Option B : Utiliser un zoom minimum (plus flexible)
        camera->zoom = std::max(optimalZoom, 2.5f); // Au moins 1x

        // Mettre à jour les bounds
        camera->setBounds(0, mapWidth, 0, mapHeight);

        std::cout << "[Game] Map size: " << mapWidth << "x" << mapHeight << "\n";
        std::cout << "[Game] Optimal zoom: " << optimalZoom << "\n";
    }

    TiledObject *musicObj = tileMapComp.getObjectByName("Music");
    if (musicObj)
    {
        std::cout << "[Game] start find a music !\n";
        if (!AudioManager::getInstance().hasLoadMusic(musicObj->properties["MusicId"]))
            AudioManager::getInstance().loadMusic(musicObj->properties["MusicId"], musicObj->properties["MusicFilePath"]);
        AudioManager::getInstance().playMusic(musicObj->properties["MusicId"], 2000);
    }

    std::cout << "[Game] map loaded successfully!\n";
}

void Game::setupSystems()
{

    // inputs
    auto *inputSys = m_manager.addSystem<PlayerInputSystem>();
    inputSys->setPriority(10);

    // collision
    auto *collisionSys = m_manager.addSystem<CollisionSystem>();
    collisionSys->setPriority(15);

    // mouvement
    auto *movementSys = m_manager.addSystem<MovementSystem>();
    movementSys->setPriority(20);

    // triggers
    auto *triggerSys = m_manager.addSystem<TriggerSystem>();
    triggerSys->setPriority(25);

    // Système d'animation
    auto *dirAnimSys = m_manager.addSystem<DirectionalAnimationSystem>();
    dirAnimSys->setPriority(30);

    // camera
    auto *cameraSys = m_manager.addSystem<CameraSystem>();
    cameraSys->setPriority(40);

    // TilemapRender layer en dessous du joueur
    auto *tilemapBeforsSys = m_manager.addSystem<TileMapRenderSystem>(0);
    tilemapBeforsSys->setPriority(99);

    // Système de rendu (toujours en dernier)
    auto *renderSys = m_manager.addSystem<RenderSystem>();
    renderSys->setPriority(100);

    // TilemapRender layyer au dessus
    auto *tilemapAfterSys = m_manager.addSystem<TileMapRenderSystem>(1);
    tilemapAfterSys->setPriority(101);

    // debug pour les hitbox (future tooltip)
    auto *debugRenderSys = m_manager.addSystem<DebugRenderSystem>(false);
    debugRenderSys->setPriority(1000);

    // les tri en fonction des priorité
    m_manager.sortSystems();
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
    player.addComponent<CollisionComponent>(0, 0, 16, 16, "Player");

    SDL_Texture *texture = loadTexture("assets/Actor/Characters/Boy/SpriteSheet.png");
    auto &sprite = player.getComponent<SpriteComponent>();
    sprite.setTexture(texture);
    sprite.renderLayer = LAYER_PLAYER;

    auto &transform = player.getComponent<TransformComponent>();
    transform.velocity = Vector2D(0, 0);
    m_player = &player;

    ECS::Entity &thisCamera = m_manager.createEntity("Camera");
    thisCamera.addComponent<CameraComponent>(800, 600);
    camera = &thisCamera.getComponent<CameraComponent>();
    camera->zoom = 2.f;

    // map
    loadMap("Spawn.tmx", "WorldSpawn");

    auto &tilemapCompo = currentMap->getComponent<TileMapComponent>();
    camera->setBounds(0, tilemapCompo.getMapWidthInPixels(), 0, tilemapCompo.getMapHeightInPixels());

    auto renderSys = m_manager.getSystem<RenderSystem>();
    renderSys->setCamera(camera);

    auto debugRenderSys = m_manager.getSystem<DebugRenderSystem>();
    debugRenderSys->setCamera(camera);

    auto triggerSys = m_manager.getSystem<TriggerSystem>();
    triggerSys->setTeleportCallback([this](const std::string &destination, const std::string &spawnPoint)
                                    { this->loadMap(destination, spawnPoint); });

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
        if (UIManager::getInstance().handleInput(m_event))
            continue;
        switch (m_event.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (m_event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                if (currentState == GameState::Playing)
                {
                    SetState(GameState::Paused);
                    UIManager::getInstance().pushMenu(new PauseMenu(m_renderer, this));
                    std::cout << "[Game] game paused\n";
                }
                break;
            }
            case SDLK_F3:
            {
                auto debugRenderSys = m_manager.getSystem<DebugRenderSystem>();
                if (debugRenderSys)
                {
                    debugRenderSys->toggle();
                }
            }
            break;
            default:
                break;
            }

        default:
            break;
        }
    }
}

void Game::Update(float deltaTime)
{
    m_deltaTime = deltaTime;

    // Update le jeu SEULEMENT si on joue
    if (currentState == GameState::Playing)
    {
        m_manager.update(m_deltaTime);
        m_manager.refresh();
    }

    // Update l'UI si des menus sont actifs
    if (UIManager::getInstance().hasMenus())
    {
        UIManager::getInstance().update(deltaTime);
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);
    if (UIManager::getInstance().hasMenus())
    {
        if (currentState == GameState::Paused)
        {
            m_manager.render(m_renderer);
        }

        UIManager::getInstance().render();
    }
    else
        m_manager.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Game::Clean()
{

    UIThemeManager::getInstance().clean();
    AudioManager::getInstance().clean();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    std::cout << "[Game] Cleaned up\n";
}

void Game::SetState(GameState newState)
{
    previousState = currentState;
    currentState = newState;
    std::cout << "[Game] State changed to: " << static_cast<int>(newState) << "\n";
}
