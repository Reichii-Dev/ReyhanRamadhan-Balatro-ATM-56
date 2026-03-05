#include "ModifierFactory.h"
#include "modifiers/DoubleScoreModifier.h"
#include "modifiers/FlatBonusModifier.h"
#include "modifiers/SquareScoreModifier.h"
#include <iostream>
#include <stdexcept>

// ============================================================
//  ModifierFactory::create
//  Factory method — maps string type to concrete modifier object
// ============================================================
std::shared_ptr<IModifier> ModifierFactory::create(const std::string& type) {
    if (type == "double") {
        return std::make_shared<DoubleScoreModifier>();
    }
    else if (type == "flat") {
        return std::make_shared<FlatBonusModifier>(50);
    }
    else if (type == "square") {
        return std::make_shared<SquareScoreModifier>();
    }
    else {
        throw std::invalid_argument("Unknown modifier type: " + type);
    }
}

void ModifierFactory::listAvailable() {
    std::cout << "  [double] Double Boost   - Multiplies score x2\n";
    std::cout << "  [flat]   Flat Boost     - Adds +50 flat points\n";
    std::cout << "  [square] Square Surge   - Squares your score\n";
}
