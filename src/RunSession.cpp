#include "RunSession.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

RunSession::RunSession()
    : currentRound_(1),
      totalRounds_(3),
      blindTarget_(100),
      blindScaling_(80),
      scoringSystem_(std::make_shared<SumScoringStrategy>())
{}

// ============================================================
//  start() — Entry point, runs the full game loop
//  Loop: Start → Play Hand → Score → Shop → Repeat → End
// ============================================================
void RunSession::start() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "\n";
    std::cout << "##############################################\n";
    std::cout << "#     CARD RUN GAME  —  Balatro-Inspired     #\n";
    std::cout << "##############################################\n";
    std::cout << "Survive " << totalRounds_ << " rounds by beating the blind score!\n\n";

    for (currentRound_ = 1; currentRound_ <= totalRounds_; ++currentRound_) {
        playRound();
    }

    std::cout << "\n##############################################\n";
    std::cout << "#         YOU COMPLETED THE RUN!             #\n";
    std::cout << "##############################################\n";
}

// ============================================================
//  playRound() — One full round: deal → score → shop
// ============================================================
void RunSession::playRound() {
    displayHeader();

    // Deal hand
    std::vector<int> hand = dealHand();
    displayHand(hand);
    displayModifiers();

    // Calculate score
    std::cout << "\n--- Scoring ---\n";
    int finalScore = scoringSystem_.calculate(hand, activeModifiers_);
    std::cout << "\n>>> FINAL SCORE: " << finalScore
              << " | TARGET: " << blindTarget_ << " <<<\n";

    // Win/lose check
    if (!checkWin(finalScore)) {
        std::cout << "\n[GAME OVER] Score too low! Better luck next time.\n";
        std::exit(0);
    }

    std::cout << "\n[ROUND " << currentRound_ << " CLEARED!]\n";

    // Scale blind for next round
    blindTarget_ += blindScaling_;

    // Shop phase (except after last round)
    if (currentRound_ < totalRounds_) {
        runShop();
        shopSystem_.refresh();
    }
}

// ============================================================
//  dealHand() — Randomly generate 5 card values (1–13)
// ============================================================
std::vector<int> RunSession::dealHand() {
    std::vector<int> hand;
    for (int i = 0; i < 5; ++i) {
        hand.push_back((std::rand() % 13) + 1);
    }
    return hand;
}

void RunSession::displayHand(const std::vector<int>& hand) const {
    std::cout << "Your hand: [ ";
    for (int v : hand) std::cout << v << " ";
    std::cout << "]\n";
}

void RunSession::displayModifiers() const {
    if (activeModifiers_.empty()) {
        std::cout << "Active modifiers: (none)\n";
        return;
    }
    std::cout << "Active modifiers: ";
    for (const auto& m : activeModifiers_) {
        std::cout << "[" << m->getName() << "] ";
    }
    std::cout << "\n";
}

// ============================================================
//  runShop() — Player buys modifier from shop
// ============================================================
void RunSession::runShop() {
    shopSystem_.displayShop();
    std::cout << "Enter modifier type to buy (or 'skip'): ";

    std::string choice;
    std::cin >> choice;

    auto modifier = shopSystem_.buyModifier(choice);
    if (modifier) {
        activeModifiers_.push_back(modifier);
        std::cout << "  Modifier added to your chain!\n";
    }
}

bool RunSession::checkWin(int score) const {
    return score >= blindTarget_;
}

void RunSession::displayHeader() const {
    std::cout << "\n----------------------------------------------\n";
    std::cout << "  ROUND " << currentRound_ << " / " << totalRounds_
              << "  |  Blind Target: " << blindTarget_ << "\n";
    std::cout << "  Scoring Rule: " << scoringSystem_.getStrategyName() << "\n";
    std::cout << "----------------------------------------------\n";
}
