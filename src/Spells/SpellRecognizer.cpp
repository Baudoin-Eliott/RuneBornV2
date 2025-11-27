// SpellRecognizer.cpp
#include "SpellRecognizer.h"
#include "SpellDatabase.h"
#include <algorithm>
#include <iostream>

RecognitionResult SpellRecognizer::recognize(
    const std::vector<int> &userPath,
    int gridLevel)
{

    RecognitionResult result;
    result.spellName = "";
    result.accuracy = 9999;
    result.isValid = false;
    result.power = 0;
    return result;
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
    const SpellPattern &spell)
{

    return 9999;
}

int SpellRecognizer::verifyPathOrder(
    const std::vector<int> &normalizedPath,
    const std::vector<int> &correctPath)
{

    return 0;
}