#include "ScoringSystem.h"
#include <algorithm>
#include <numeric>
#include <iostream>

// ============================================================
//  SumScoringStrategy — adds all card values together
// ============================================================
int SumScoringStrategy::calculateBase(const std::vector<int>& handValues) const {
    return std::accumulate(handValues.begin(), handValues.end(), 0);
}

// ============================================================
//  HighCardScoringStrategy — only counts the highest card
// ============================================================
int HighCardScoringStrategy::calculateBase(const std::vector<int>& handValues) const {
    if (handValues.empty()) return 0;
    return *std::max_element(handValues.begin(), handValues.end());
}

// ============================================================
//  ScoringSystem
// ============================================================
ScoringSystem::ScoringSystem(std::shared_ptr<IScoringStrategy> strategy)
    : strategy_(std::move(strategy)) {}

int ScoringSystem::calculate(const std::vector<int>& handValues,
                              const std::vector<std::shared_ptr<IModifier>>& modifiers) const {
    // Step 1: Get base score from strategy
    int score = strategy_->calculateBase(handValues);
    std::cout << "  Base score (" << strategy_->getRuleName() << "): " << score << "\n";

    // Step 2: Apply each modifier in chain (Decorator-like)
    for (const auto& mod : modifiers) {
        int before = score;
        score = mod->apply(score);
        std::cout << "  [" << mod->getName() << "] " << before << " -> " << score << "\n";
    }

    return score;
}

void ScoringSystem::setStrategy(std::shared_ptr<IScoringStrategy> strategy) {
    strategy_ = std::move(strategy);
}

std::string ScoringSystem::getStrategyName() const {
    return strategy_->getRuleName();
}
