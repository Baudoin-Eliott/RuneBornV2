#pragma once

#include <vector>
#include <string>

struct SpellPattern {

    std::string name;
    std::string effectClass;
    int level;

    std::vector<std::vector<int>> costMap;
    std::vector<int> correctPath;
    int baseDamage;
    int speed;
    int lifetime;

    int getCostBy1DId(int id) {return costMap[id / level][id % level];};
};