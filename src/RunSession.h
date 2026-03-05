#pragma once
#include <vector>
#include <memory>
#include "ScoringSystem.h"
#include "ShopSystem.h"
#include "modifiers/IModifier.h"

// ============================================================
//  RunSession — Main game loop controller
//  Manages: rounds, hands, modifiers, scoring, shop
//  Pattern: Uses Strategy (via ScoringSystem) + Factory (via Shop)
// ============================================================
class RunSession {
public:
    RunSession();

    // Start and run the full game session
    void start();

private:
    // --- Game state ---
    int currentRound_;
    int totalRounds_;
    int blindTarget_;        // Score needed to pass each round
    int blindScaling_;       // How much target increases per round

    // --- Systems ---
    ScoringSystem scoringSystem_;
    ShopSystem shopSystem_;

    // --- Active modifiers (Decorator chain) ---
    std::vector<std::shared_ptr<IModifier>> activeModifiers_;

    // --- Private helpers ---
    void playRound();
    std::vector<int> dealHand();
    void displayHand(const std::vector<int>& hand) const;
    void displayModifiers() const;
    void runShop();
    bool checkWin(int score) const;
    void displayHeader() const;
};
