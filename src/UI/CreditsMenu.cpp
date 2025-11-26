#include "CreditsMenu.h"

int CreditsMenu::getThumbPosition() const
{
    if (minY >= 0)
        return scrollbarY;

    float scrollRange = abs(minY);
    float scrollProgress = -posY / scrollRange;

    int availableSpace = scrollbarHeight - thumbHeight;
    int thumbY = scrollbarY + (availableSpace * scrollProgress);

    return thumbY;
}

void CreditsMenu::onEnter()
{
    Menu::onEnter();
    if (!AudioManager::getInstance().hasLoadSound("MenuAccept"))
        AudioManager::getInstance().loadSound("MenuAccept", {"Sounds/Menu/Accept.wav", "Sounds/Menu/Accept2.wav", "Sounds/Menu/Accept3.wav", "Sounds/Menu/Accept4.wav", "Sounds/Menu/Accept5.wav", "Sounds/Menu/Accept6.wav", "Sounds/Menu/Accept7.wav", "Sounds/Menu/Accept8.wav", "Sounds/Menu/Accept9.wav"});
    if (!AudioManager::getInstance().hasLoadSound("MenuCancel"))
        AudioManager::getInstance().loadSound("MenuCancel", {"Sounds/Menu/Cancel.wav", "Sounds/Menu/Cancel2.wav"});
    if (!AudioManager::getInstance().hasLoadSound("Menu"))
        AudioManager::getInstance().loadSound("Menu", {"Sounds/Menu/Menu5.wav", "Sounds/Menu/Menu6.wav", "Sounds/Menu/Menu8.wav", "Sounds/Menu/Menu10.wav", "Sounds/Menu/Menu11.wav", "Sounds/Menu/Menu12.wav"});
    if (!AudioManager::getInstance().hasLoadSound("MenuMove"))
        AudioManager::getInstance().loadSound("MenuMove", {"Sounds/Menu/Move1.wav", "Sounds/Menu/Move2.wav", "Sounds/Menu/Move3.wav", "Sounds/Menu/Move4.wav", "Sounds/Menu/Move5.wav", "Sounds/Menu/Move6.wav", "Sounds/Menu/Move7.wav", "Sounds/Menu/Move8.wav"});
}

bool CreditsMenu::handleInput(SDL_Event &event)
{
    if (event.type == SDL_MOUSEWHEEL)
    {
        if (event.wheel.y < 0)
        {
            posY -= 40;
            if (posY < minY)
                posY = minY;
        }
        else if (event.wheel.y > 0)
        {
            posY += 40;
            if (posY > 0)
                posY = 0;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            if (isMouseInRect(event.button.x, event.button.y, {scrollbarX, getThumbPosition(), scrollbarWidth, thumbHeight}))
            {
                isDraggingScrollbar = true;
            }
            if (isMouseInRect(event.button.x, event.button.y, buttonRects[selected]))
            {
                AudioManager::getInstance().playSound("MenuAccept");
                AudioManager::getInstance().playSound("MenuAccept");
                UIManager::getInstance().popMenu();
                game->SetState(game->previousState);
                return true;
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            isDraggingScrollbar = false;
        }
    }

    if (event.type == SDL_MOUSEMOTION)
    {
        if (isDraggingScrollbar)
        {
            int relativeY = event.motion.y - scrollbarY - (thumbHeight / 2);
            int availableSpace = scrollbarHeight - thumbHeight;
            float scrollRatio = (float)relativeY / availableSpace;

            posY = -(scrollRatio * abs(minY));

            if (posY > 0)
                posY = 0;
            if (posY < minY)
                posY = minY;

            return true;
        }
    }

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case (SDLK_UP):
        case (SDLK_z):
            posY += 20;
            if (posY > 0)
                posY = 0;
            break;

        case (SDLK_DOWN):
        case (SDLK_s):
            posY -= 20;
            if (posY < minY)
                posY = minY;
            break;
        case (SDLK_RETURN):
        case (SDLK_SPACE):
        case (SDLK_ESCAPE):
            AudioManager::getInstance().playSound("MenuAccept");
            UIManager::getInstance().popMenu();
            game->SetState(game->previousState);
            return true;

        default:
            return false;
        }
    }
    return false;
}

void CreditsMenu::render(SDL_Renderer *rend)
{

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend, 25, 25, 25, 255);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(rend, &overlay);

    UIThemeManager::getInstance().renderPanel(250, 30, 300, 80);

    UIThemeManager::getInstance().renderTextCentered("RUNEBORN", 400, 70, UIThemeManager::getInstance().getCurrentColor());
    // Options du menu
    int Y = 160 + posY;
    int spacing = 60;

    for (size_t i = 0; i < titles.size(); i++)
    {
        if (Y > 110 && Y < 600)
        {
            UIThemeManager::getInstance().renderTextCentered(titles[i], 400, Y, {255, 255, 255, 255});
        }
        Y += 30;

        if (Y > 110 && Y < 600)
        {
            UIThemeManager::getInstance().renderTextCentered(texts[i], 400, Y, {255, 255, 255, 255});
        }
        Y += 50;
    }
    SDL_Rect buttonRect = {250, Y - 5, 300, 50};
    buttonRects.clear();
    buttonRects.push_back(buttonRect);
    UIThemeManager::getInstance().renderPanel(250, Y - 5, 300, 50);
    UIThemeManager::getInstance().renderTextCentered("Return", 400, Y + 20, UIThemeManager::getInstance().getCurrentSelectedColor());

    // slider

    UIThemeManager::getInstance().renderPanel(scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight, "assets/Ui/Theme/Theme Wood/nine_path_bg_2.png");

    if (isDraggingScrollbar)
    {
        if (getThumbPosition() - scrollbarY > 1)
            UIThemeManager::getInstance().renderPanel(scrollbarX, scrollbarY, scrollbarWidth, getThumbPosition() - scrollbarY, "assets/Ui/Theme/Theme Wood/slider_progress_hover.png");
        UIThemeManager::getInstance().renderPanel(scrollbarX, getThumbPosition(), scrollbarWidth, thumbHeight, "assets/Ui/Theme/Theme Wood/v_slidder_grabber.png");
    }
    else
    {
        if (getThumbPosition() - scrollbarY > 1)
            UIThemeManager::getInstance().renderPanel(scrollbarX, scrollbarY, scrollbarWidth, getThumbPosition() - scrollbarY, "assets/Ui/Theme/Theme Wood/slider_progress_hover.png");
        UIThemeManager::getInstance().renderPanel(scrollbarX, getThumbPosition(), scrollbarWidth, thumbHeight, "assets/Ui/Theme/Theme Wood/slider_progress.png");
    }
}
