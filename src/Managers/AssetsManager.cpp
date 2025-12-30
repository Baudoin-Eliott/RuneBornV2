#include "AssetsManager.h"
#include <iostream>
#include <SDL2/SDL_image.h>


SDL_Texture* AssetsManager::getTexture(SDL_Renderer* rend, const std::string& filePath){

    std::string fullPath = "assets/" + filePath;

    if (textures.find(fullPath) != textures.end()){
        return textures[fullPath];
    }

    SDL_Surface* surface = IMG_Load(fullPath.c_str());
    if (!surface) {
        std::cerr << "[AssetsManager] Failed to load image: " << fullPath << " SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "[AssetsManager] Failed to create texture from surface: " << fullPath << " SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    textures[fullPath] = texture;
    return texture;
}

void AssetsManager::clearTextures() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}

AssetsManager::~AssetsManager() {
    clearTextures();
}
