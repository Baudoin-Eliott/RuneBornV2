
#pragma once
#include "SpellPattern.h"
#include <vector>
#include <string>


struct RecognitionResult {
    std::string spellName;
    int accuracy;   
    bool isValid;     
    int power;           
};

class SpellRecognizer {
private:

    struct NormalizedPattern {
        std::vector<int> points;
        int size;
    };

public:

    static RecognitionResult recognize(
        const std::vector<int>& userPath, 
        int gridLevel
    );
    
private:

    static NormalizedPattern normalizePattern(
        const std::vector<int>& pattern, 
        int originalLevel
    );
    

    static int calculateScore(
        const std::vector<int>& normalizedPath,
        const SpellPattern& spell
    );
    
    static int verifyPathOrder(
        const std::vector<int>& normalizedPath,
        const std::vector<int>& correctPath
    );
};