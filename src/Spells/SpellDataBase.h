#pragma once
#include "SpellPattern.h"
#include <vector>
#include <string>
#include <unordered_map>

class SpellDataBase
{

private:
    static std::unordered_map<int, std::vector<SpellPattern>> patterns;
    static bool isLoaded;

public:
    static bool loadFromFile(const std::string &filepath);

    static const std::vector<SpellPattern> &getPatternsForLevel(int level);

    static const SpellPattern *getPatternByName(const std::string &name);
};