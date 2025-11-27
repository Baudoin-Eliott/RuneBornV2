#include "SpellFactory.h"

#include "Effects/FireBallEffect.h"

std::unordered_map<std::string, SpellFactory::CreatorFunc> SpellFactory::creators;

std::unique_ptr<SpellEffect> SpellFactory::create(const SpellPattern& pattern, int power){

    auto it = creators.find(pattern.effectClass);
    if (it == creators.end()){
        std::cerr << "Unknown spell effect: "<< pattern.effectClass << " try registerSpell befors!\n";
        return nullptr;
    }

    return it->second(pattern, power);
}

void SpellFactory::initializeSpells() {

    //ici tous les sorts que j'ai cree
    registerSpell<FireBallEffect>("FireBallEffect");
    
    std::cout << "[SpellFactory] Registered " << creators.size() << " spell types\n";
}