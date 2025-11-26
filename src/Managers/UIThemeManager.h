#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include <iostream>

class UIThemeManager
{
private:
    UIThemeManager() = default;
    UIThemeManager(const UIThemeManager &) = delete;
    UIThemeManager &operator=(const UIThemeManager &) = delete;

    SDL_Renderer *renderer = nullptr;

    TTF_Font *font = nullptr;
    int fontSize = 24;

    std::unordered_map<std::string, SDL_Texture *> themePanels;
    std::unordered_map<std::string, SDL_Texture *> customTextureCache;
    std::string currentTheme = "ThemeDark";

    int cornerSize = 16;
    SDL_Color currentColor = {200, 200, 200, 255};
    SDL_Color currentSelectedColor = {255, 255, 0, 255};
    bool initialized = false;

public:
    static UIThemeManager &getInstance();

    bool init(SDL_Renderer *rend);

    SDL_Color getCurrentColor() { return currentColor; }

    void setCurrentColor(SDL_Color newCol) { currentColor = newCol; }

    SDL_Color getCurrentSelectedColor() { return currentSelectedColor; }

    void setCurrentSelectedColor(SDL_Color newCol) { currentSelectedColor = newCol; }

    void clean();

    bool loadFont(const std::string &fontPath, int size);

    bool loadTheme(const std::string &themeName);

    void setTheme(const std::string &themeName);

    std::string getCurrentTheme() { return currentTheme; }

    void renderText(const std::string &text, int x, int y, SDL_Color color = {255, 255, 255});

    void renderTextCentered(const std::string &text, int centerX, int centerY, SDL_Color color = {255, 255, 255, 255});
    void renderPanel(int x, int y, int w, int h, const std::string &customName = "");
    ~UIThemeManager();
    int getCornerSize() const { return cornerSize; }
    void setCornerSize(int size) { cornerSize = size; }
};