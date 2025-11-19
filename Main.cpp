#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {

    // Configuration du jeu
    const int FPS = 60;
    const float TARGET_DELTA = 1000.0f / FPS;  // En millisecondes

    // Création du jeu
    Game* game = new Game();

    if (game->Init("RuneBorn V2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, false) != 0) {
        std::cerr << "[Main] Failed to initialize game!\n";
        delete game;
        return 1;
    }

    std::cout << "[Main] Game loop starting...\n";

    // Variables pour le timing
    Uint64 lastTime = SDL_GetTicks();

    // Boucle principale du jeu
    while (game->isRunning()) {

        // Calcul du delta time
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;  // Convertir en secondes
        lastTime = currentTime;

        // Mise à jour du jeu
        game->HandleEvents();
        game->Update(deltaTime);  // Passer le deltaTime au nouveau ECS
        game->Render();

        // Limitation du framerate
        Uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < TARGET_DELTA) {
            SDL_Delay(static_cast<Uint32>(TARGET_DELTA - frameTime));
        }
    }

    // Nettoyage
    game->Clean();
    delete game;

    std::cout << "[Main] Game exited cleanly\n";
    return 0;
}