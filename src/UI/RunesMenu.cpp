#include "RunesMenu.h"
#include "../Managers/AudioManager.h"
#include "../Managers/UIThemeManager.h"
#include "../../ECS/Utils/UIManager.h"
#include "../../ECS/Utils/GameState.h"
#include "../Spells/SpellRecognizer.h"

RunesMenu::RunesMenu(SDL_Renderer *rend, Game *g, int dif) : Menu("RunesMenu"), renderer(rend), game(g), level(dif), isCasting(false)
{
    spacingX = static_cast<int>(550.0f / (level - 1) + 0.5f);
    spacingY = static_cast<int>(350.0f / (level - 1) + 0.5f);
    tolerance = (spacingX / 4 + spacingY / 4) / 2;
}

void RunesMenu::onEnter()
{
    // son de tictac
}

bool RunesMenu::handleInput(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && isCasting)
    {

        SpellRecognizer::recognize(connections, level);
        AudioManager::getInstance().playSound("MenuCancel");
        game->SetState(GameState::Playing);
        UIManager::getInstance().popMenu();
        return true;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !isCasting)
    {

        int point = calculateNearestPoint({(float)event.button.x, (float)event.button.y});
        if (point == -1)
            return true;
        isCasting = true;
        connections.clear();
        connections.push_back(point);
    }

    if (event.type == SDL_MOUSEMOTION && isCasting)
    {
        lastMousePos = {(float)event.motion.x, (float)event.motion.y};
        int point = calculateNearestPoint(lastMousePos);
        if (connections.size() > 1 && point == connections[0])
        {
            SpellRecognizer::recognize(connections, level);
            AudioManager::getInstance().playSound("MenuCancel");
            game->SetState(GameState::Playing);
            UIManager::getInstance().popMenu();
            return true;
        }
        if (connections.back() != point && point != -1 && !haveConnect(point))
            connections.push_back(point);
    }
    return false;
}

void RunesMenu::render(SDL_Renderer *rend)
{
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend, 24, 24, 24, 128);
    SDL_Rect overlay = {100, 100, 600, 400};
    SDL_RenderFillRect(rend, &overlay);
    int spacing = 125;
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (int i = 0; i < level; i++)
    {
        for (int j = 0; j < level; j++)
        {
            SDL_Rect pointRect = {spacing + i * spacingX - 2, spacing + j * spacingY - 2, 3, 3};
            SDL_RenderFillRect(renderer, &pointRect);
        }
    }

    if (!connections.empty())
    {

        SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
        for (int i = 0; i < connections.size() - 1; i++)
        {
            Vector2D point1 = getPosById(connections[i]);
            Vector2D point2 = getPosById(connections[i + 1]);
            SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
        }
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        Vector2D lastPointPos = getPosById(connections.back());
        SDL_RenderDrawLine(renderer, lastPointPos.x, lastPointPos.y, lastMousePos.x, lastMousePos.y);
    }
}

void RunesMenu::onExit()
{
    for (int id : connections)
    {
        std::cout << id << " ";
    }
}

int RunesMenu::calculateNearestPoint(Vector2D pos)
{

    int gridX = static_cast<int>((pos.x - 125.0f) / spacingX + 0.5f);
    int gridY = static_cast<int>((pos.y - 125.0f) / spacingY + 0.5f);

    if (gridX < 0 || gridX >= level || gridY < 0 || gridY >= level)
    {
        return -1;
    }

    Vector2D pointPos = getPosById(gridX + gridY * level);

    if (pos.Distance(pointPos) < tolerance)
        return gridX + gridY * level;
    return -1;
}

Vector2D RunesMenu::getPosById(int id)
{
    int pointPosX = 125 + (id % level) * spacingX;
    int pointPosY = 125 + (id / level) * spacingY;

    return {(float)pointPosX, (float)pointPosY};
}

bool RunesMenu::haveConnect(int point)
{
    for (int id : connections)
    {
        if (id == point)
            return true;
    }
    return false;
}
