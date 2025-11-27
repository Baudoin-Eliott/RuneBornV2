#include "SpellDatabase.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>


using json = nlohmann::json; 

std::unordered_map<int, std::vector<SpellPattern>> SpellDataBase::patterns;
bool SpellDataBase::isLoaded = false;

bool SpellDataBase::loadFromFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "[SpellDataBase] Error while opening file: " << filepath << " !\n";
        return false;
    }

    json data;
    try
    {
        file >> data;
    }
    catch (const json::exception &e)
    {
        std::cerr << "[SpellDataBase] Error json: " << e.what() << "\n";
        return false;
    }

    for (auto &[levelKey, spells] : data.items())
    {
        if (levelKey.substr(0, 3) != "Lvl")
            continue;
        int level = std::stoi(levelKey.substr(3));

        for (auto &[spellName, spellData] : spells.items())
        {
            SpellPattern pattern;
            pattern.name = spellName;
            pattern.level = level;
            pattern.correctPath = spellData["value"].get<std::vector<int>>();
            pattern.costMap = spellData["pattern"].get<std::vector<std::vector<int>>>();
            pattern.effectClass = spellData["EffectClass"];

            patterns[level].push_back(pattern);
        }
    }

    isLoaded = true;
    return true;
}

const std::vector<SpellPattern> &SpellDataBase::getPatternsForLevel(int level)
{
    auto it = patterns.find(level);
    if (it != patterns.end())
        return it->second;

    static std::vector<SpellPattern> empty;
    return empty;
}

const SpellPattern *SpellDataBase::getPatternByName(const std::string &name)
{
    for (auto& [level, spellList] : patterns){
        for (auto& pattern : spellList){
            if (pattern.name == name)
            return &pattern;
        }
    }
    return nullptr;
}
