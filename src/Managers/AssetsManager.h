
#pragma once
#include <SDL2/SDL.h>

#include <map>
#include <string>

class AssetsManager {
private:

    std::map<std::string, SDL_Texture*> textures;

    AssetsManager() = default;
    ~AssetsManager();

public:

    static AssetsManager& getInstance() {
        static AssetsManager instance;
        return instance;
    }

    SDL_Texture* getTexture(SDL_Renderer* rend, const std::string& filePath);


    void clearTextures();
};