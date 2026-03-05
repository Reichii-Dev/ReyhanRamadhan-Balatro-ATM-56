#pragma once
#include "IModifier.h"

// ============================================================
//  FlatBonusModifier — Adds a fixed flat bonus to the score
//  Pattern: Concrete Decorator
// ============================================================
class FlatBonusModifier : public IModifier {
public:
    explicit FlatBonusModifier(int bonus = 50) : bonus_(bonus) {}

    int apply(int currentScore) const override {
        return currentScore + bonus_;
    }

    std::string getName() const override {
        return "Flat Boost";
    }

    std::string getDescription() const override {
        return "Adds +" + std::to_string(bonus_) + " flat points to score";
    }

private:
    int bonus_;
};
