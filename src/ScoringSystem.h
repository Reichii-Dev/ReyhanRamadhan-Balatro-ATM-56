#pragma once
#include <vector>
#include <memory>
#include <string>
#include "modifiers/IModifier.h"

// ============================================================
//  ScoringSystem — Calculates score from hand + modifiers
//  Pattern: Strategy (Behavioral)
//  The scoring "rule" (base score calculation) is encapsulated
//  and can be swapped independently from modifier logic.
// ============================================================

// --- Strategy Interface ---
class IScoringStrategy {
public:
    virtual ~IScoringStrategy() = default;
    virtual int calculateBase(const std::vector<int>& handValues) const = 0;
    virtual std::string getRuleName() const = 0;
};

// --- Concrete Strategy: Sum all card values ---
class SumScoringStrategy : public IScoringStrategy {
public:
    int calculateBase(const std::vector<int>& handValues) const override;
    std::string getRuleName() const override { return "Sum Score"; }
};

// --- Concrete Strategy: Highest card only ---
class HighCardScoringStrategy : public IScoringStrategy {
public:
    int calculateBase(const std::vector<int>& handValues) const override;
    std::string getRuleName() const override { return "High Card Score"; }
};

// --- ScoringSystem: uses a strategy + applies modifier chain ---
class ScoringSystem {
public:
    explicit ScoringSystem(std::shared_ptr<IScoringStrategy> strategy);

    // Calculate final score: base score → apply each modifier in chain
    int calculate(const std::vector<int>& handValues,
                  const std::vector<std::shared_ptr<IModifier>>& modifiers) const;

    void setStrategy(std::shared_ptr<IScoringStrategy> strategy);
    std::string getStrategyName() const;

private:
    std::shared_ptr<IScoringStrategy> strategy_;
};
