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
    static UIThemeManager &getInstance()
    {
        static UIThemeManager instance;
        return instance;
    }

    bool init(SDL_Renderer *rend)
    {
        if (initialized)
            return true;

        renderer = rend;

        if (TTF_Init() == -1)
        {
            std::cerr << "[UIThemeManager] error while initialization of SDL2_tff\n";
            return false;
        }

        if (!loadFont("assets/Ui/Font/NormalFont.ttf", 24))
        {
            std::cerr << "[UIThemeManager] Error while loading font at assets/Ui/Font/NormalFont.ttf\n";
            return false;
        }

        if (!loadTheme(currentTheme))
        {
            std::cerr << "[UIThemeManager] Error while loading theme" << currentTheme << "\n";
            return false;
        }

        initialized = true;
        return true;
    }

    SDL_Color getCurrentColor()
    {
        return currentColor;
    }

    void setCurrentColor(SDL_Color newCol)
    {
        currentColor = newCol;
    }

    SDL_Color getCurrentSelectedColor()
    {
        return currentSelectedColor;
    }

    void setCurrentSelectedColor(SDL_Color newCol)
    {
        currentSelectedColor = newCol;
    }

    void clean()
    {

        if (font)
        {
            TTF_CloseFont(font);
            font = nullptr;
        }

        for (auto &pair : themePanels)
        {
            if (pair.second)
                SDL_DestroyTexture(pair.second);
        }
        themePanels.clear();

        TTF_Quit();
        initialized = false;
    }

    bool loadFont(const std::string &fontPath, int size)
    {
        if (font)
            TTF_CloseFont(font);

        font = TTF_OpenFont(fontPath.c_str(), size);

        if (!font)
        {
            std::cerr << "[UIThemeManager] error while loading font at: " << fontPath << "\n";
            return false;
        }
        fontSize = size;

        std::cout << "[UIThemeManager] font laoded !\n";
        return true;
    }

    bool loadTheme(const std::string &themeName)
    {
        if (themePanels.find(themeName) != themePanels.end())
        {
            currentTheme = themeName;
            return true;
        }

        std::string path = "assets/Ui/Theme/Wip/" + themeName + "/nine_path_panel.png";

        SDL_Surface *surface = IMG_Load(path.c_str());
        if (!surface)
        {
            std::cerr << "[UIThemeManager] Failed to load theme: " << path
                      << "\n  IMG_Error: " << IMG_GetError() << "\n";
            return false;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture)
        {
            std::cerr << "[UIThemeManager] Error while creating texture from surface !\n";
            return false;
        }

        themePanels[themeName] = texture;
        currentTheme = themeName;
        return true;
    }

    void setTheme(const std::string &themeName)
    {
        if (!loadTheme(themeName))
        {
            std::cerr << "[UIThemeManager] failed to set theme to: " << themeName << "\n";
        }
    }

    std::string getCurrentTheme()
    {
        return currentTheme;
    }

    void renderText(const std::string &text, int x, int y, SDL_Color color = {255, 255, 255})
    {
        if (!font)
        {
            std::cerr << "[UIThemeManager] error, no font found !\n";
            return;
        }

        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (!surface)
        {
            std::cerr << "[UIThemeManager] Error while creating surface from text !\n";
            return;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect dstRect = {x, y, surface->w, surface->h};

        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    void renderTextCentered(const std::string &text, int centerX, int centerY, SDL_Color color = {255, 255, 255, 255})
    {
        if (!font)
            return;

        int textW, textH;
        TTF_SizeText(font, text.c_str(), &textW, &textH);

        renderText(text, centerX - textW / 2, centerY - textH / 2, color);
    }

    void renderPanel(int x, int y, int w, int h, const std::string &customName = "")
    {
        SDL_Texture *panelTexture = nullptr;

        if (customName == "")
        {

            auto it = themePanels.find(currentTheme);
            if (it == themePanels.end())
            {
                std::cerr << "[UIThemeManager] Current Theme not loaded current theme: " << currentTheme << "\n";
                return;
            }
            panelTexture = it->second;
        }
        else
        {
            auto it = customTextureCache.find(customName);
             if (it != customTextureCache.end())
            {
                panelTexture = it->second;
            }
            else
            {
                SDL_Surface *surface = IMG_Load(customName.c_str());
                if (!surface)
                {
                    std::cerr << "[UIThemeManager] Error while loading surface from: " << customName << "\n";
                    return;
                }

                panelTexture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                
                if (!panelTexture)
                {
                    std::cerr << "[UIThemeManager] Error creating texture from: " << customName << "\n";
                    return;
                }
                customTextureCache[customName] = panelTexture;

            }
        }

        int texW, texH;
        SDL_QueryTexture(panelTexture, nullptr, nullptr, &texW, &texH);

        int regionW = texW / 3;
        int regionH = texH / 3;

        SDL_Rect src = {0, 0, regionW, regionH};
        SDL_Rect dst = {x, y, regionW, regionH};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {regionW * 2, 0, regionW, regionH};
        dst = {x + w - regionW, y, regionW, regionH};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {0, regionH * 2, regionW, regionH};
        dst = {x, y + h - regionH, regionW, regionH};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {regionW * 2, regionH * 2, regionW, regionH};
        dst = {x + w - regionW, y + h - regionH, regionW, regionH};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {regionW, 0, regionW, regionH};
        dst = {x + regionW, y, w - regionW * 2, regionH};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {regionW, regionH * 2, regionW, regionH};
        dst = {x + regionW, y + h - regionH, w - regionW * 2, regionH};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {0, regionH, regionW, regionH};
        dst = {x, y + regionH, regionW, h - regionH * 2};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {regionW * 2, regionH, regionW, regionH};
        dst = {x + w - regionW, y + regionH, regionW, h - regionH * 2};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);

        src = {regionW, regionH, regionW, regionH};
        dst = {x + regionW, y + regionH, w - regionW * 2, h - regionH * 2};
        SDL_RenderCopy(renderer, panelTexture, &src, &dst);
    }

    ~UIThemeManager()
    {

        for (auto &pair : themePanels)
        {
            if (pair.second)
            {
                SDL_DestroyTexture(pair.second);
            }
        }
        themePanels.clear();
        for (auto &pair : customTextureCache){
            if (pair.second){
                SDL_DestroyTexture(pair.second);
            }
        }
        customTextureCache.clear();

        TTF_CloseFont(font);
        font = nullptr;
    }

    int getCornerSize() const { return cornerSize; }
    void setCornerSize(int size) { cornerSize = size; }
};