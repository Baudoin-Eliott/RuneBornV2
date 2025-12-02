// SpellRecognizer.cpp
#include "SpellRecognizer.h"
#include "SpellDatabase.h"
#include <algorithm>
#include <iostream>

int findPosition(const std::vector<int> &vec, int value)
{
    for (size_t i = 0; i < vec.size(); i++)
        if (vec[i] == value)
            return i;
    return -1;
}

int calculateDistance(int cellA, int cellB, int gridSize)
{

    int xA = cellA / gridSize;
    int yA = cellA % gridSize;

    int xB = cellB / gridSize;
    int yB = cellB % gridSize;

    return abs(xA - xB) + abs(yA - yB);
}

int countInversions(const std::vector<int> &normalizedPath, const std::vector<int> &correctPath)
{

    int inversion = 0;

    for (size_t i = 0; i < correctPath.size(); i++)
    {
        for (size_t j = i + 1; j < correctPath.size(); j++)
        {
            int valueA = correctPath[i];
            int valueB = correctPath[j];

            int posA = findPosition(normalizedPath, valueA);
            int posB = findPosition(normalizedPath, valueB);

            if (posA == -1 || posB == -1)
            {
                continue;
            }
            if (posA > posB)
            {
                inversion++;
            }
        }
    }
    return inversion;
}

int spacialPenalty(const std::vector<int> &normalizedPath, const std::vector<int> &correctPath, int gridSize)
{
    int nbPenalty = 0;
    for (size_t i = 0; i < normalizedPath.size(); i++)
    {
        int minDist = INT_MAX;
        for (size_t j = 0; j < correctPath.size(); j++)
        {
            int dist = calculateDistance(normalizedPath[i], correctPath[j], gridSize);
            if (dist < minDist)
                minDist = dist;
        }
        if (minDist > 1)
            nbPenalty += minDist - 1;
    }
    return nbPenalty;
}

int missingPoint(const std::vector<int> &normalizedPath, const std::vector<int> &correctPath, int gridSize)

{

    int missingPoints = 0;
    for (size_t i = 0; i < correctPath.size(); i++)
    {
        int minDist = INT_MAX;
        for (size_t j = 0; j < normalizedPath.size(); j++)
        {
            int dist = calculateDistance(correctPath[i], normalizedPath[j], gridSize);
            if (dist < minDist)
                minDist = dist;
        }
        if (minDist > 1)
        {
            missingPoints++;
        }
    }
    return missingPoints;
}

RecognitionResult SpellRecognizer::recognize(
    const std::vector<int> &userPath,
    int gridLevel)
{

    RecognitionResult bestResult = {"", INT_MAX, false, 0};
    NormalizedPattern normalizedPath = normalizePattern(userPath, gridLevel);

    std::vector<SpellPattern> spells = SpellDataBase::getPatternsForLevel(normalizedPath.size);

    for (auto &spell : spells)
    {
        int score = calculateScore(normalizedPath.points, spell);
        score += verifyPathOrder(normalizedPath.points, spell.correctPath, normalizedPath.size);
        if (score < bestResult.accuracy)
        {
            bestResult.spellName = spell.name;
            bestResult.accuracy = score;
        }
    }

    bestResult.isValid = true;
    if (bestResult.accuracy < 10)
        bestResult.power = 120;
    else if (bestResult.accuracy < 30)
        bestResult.power = 100;
    else if (bestResult.accuracy < 50)
        bestResult.power = 80;
    else if (bestResult.accuracy < 70)
        bestResult.power = 60;
    else if (bestResult.accuracy < 90)
        bestResult.power = 40;
    else
    {
        bestResult.isValid = false;
        bestResult.power = 0;
    }

    if (bestResult.isValid)
    {
        std::cout << "[SpellRecognizer] Recognized: " << bestResult.spellName
                  << " (accuracy: " << bestResult.accuracy
                  << ", power: " << bestResult.power << "%)\n";
    }
    else
    {
        std::cout << "[SpellRecognizer] Failed to recognize spell (accuracy: "
                  << bestResult.accuracy << ")\n";
    }

    return bestResult;
}

SpellRecognizer::NormalizedPattern SpellRecognizer::normalizePattern(
    const std::vector<int> &pattern,
    int originalLevel)
{
    if (pattern.empty())
    {
        return {std::vector<int>(), 0};
    }

    int xmin = INT_MAX, xmax = INT_MIN;
    int ymin = INT_MAX, ymax = INT_MIN;

    for (int id : pattern)
    {
        int x = id / originalLevel;
        int y = id % originalLevel;

        xmin = std::min(xmin, x);
        xmax = std::max(xmax, x);
        ymin = std::min(ymin, y);
        ymax = std::max(ymax, y);
    }

    int newSize = std::max(xmax - xmin, ymax - ymin) + 1;

    std::vector<int> normalized;
    normalized.reserve(pattern.size());

    for (int id : pattern)
    {
        int x = (id / originalLevel) - xmin;
        int y = (id % originalLevel) - ymin;
        normalized.push_back(x * newSize + y);
    }

    return {normalized, newSize};
}

int SpellRecognizer::calculateScore(
    const std::vector<int> &normalizedPath,
    SpellPattern &spell)
{
    int totalCost = 0;
    for (int pointId : normalizedPath)
    {
        totalCost += spell.getCostBy1DId(pointId);
    }
    return totalCost;
}

int SpellRecognizer::verifyPathOrder(
    const std::vector<int> &normalizedPath,
    const std::vector<int> &correctPath,
    int gridSize)
{
    int penalty = 0;
    // point en plus / moins
    if (normalizedPath.size() - correctPath.size() != 0)
        penalty += (abs(normalizedPath.size() - correctPath.size())) * (normalizedPath.size() - correctPath.size() > 0 ? 10 : 20);

    // si il y a des inversion
    penalty += countInversions(normalizedPath, correctPath) * 40;

    // les ecart spacial
    penalty += spacialPenalty(normalizedPath, correctPath, gridSize) * 20;

    // les points que l'on a absolument pas visité
    penalty += missingPoint(normalizedPath, correctPath, gridSize) * 40;

    return penalty;
}
