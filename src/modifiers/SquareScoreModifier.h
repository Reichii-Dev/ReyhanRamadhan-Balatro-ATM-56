#pragma once
#include "IModifier.h"

// ============================================================
//  SquareScoreModifier — Squares the score (score * score)
//  Pattern: Concrete Decorator
//  NOTE: Added later to demonstrate extensibility (see analysis.md A2)
// ============================================================
class SquareScoreModifier : public IModifier {
public:
    int apply(int currentScore) const override {
        return currentScore * currentScore;
    }

    std::string getName() const override {
        return "Square Surge";
    }

    std::string getDescription() const override {
        return "Squares your score (score x score) — extremely powerful!";
    }
};
