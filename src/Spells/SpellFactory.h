#pragma once
#include "SpellEffect.h"
#include "SpellPattern.h"
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <ECS.h>

class SpellFactory
{

private:
    using CreatorFunc = std::function<std::unique_ptr<SpellEffect>(const SpellPattern &, int)>;
    static std::unordered_map<std::string, CreatorFunc> creators;

    public:

    template<typename T>
    static void registerSpell(const std::string& className){
        creators[className] = [](const SpellPattern& pattern, int power){
            return std::make_unique<T>(pattern, power);
        };
    }


    static std::unique_ptr<SpellEffect> create(const SpellPattern& pattern, int power);

    static void initializeSpells();


};