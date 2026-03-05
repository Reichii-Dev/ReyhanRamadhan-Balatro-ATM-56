#pragma once
#include "IModifier.h"

// ============================================================
//  DoubleScoreModifier — Multiplies score by 2
//  Pattern: Concrete Decorator
// ============================================================
class DoubleScoreModifier : public IModifier {
public:
    int apply(int currentScore) const override {
        return currentScore * 2;
    }

    std::string getName() const override {
        return "Double Boost";
    }

    std::string getDescription() const override {
        return "Doubles your current score (x2)";
    }
};
