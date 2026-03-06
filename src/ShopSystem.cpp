#include "ShopSystem.h"
#include <iostream>

ShopSystem::ShopSystem() : playerGold_(3), itemCost_(1) {}

void ShopSystem::displayShop() const {
    std::cout << "\n========== SHOP ==========\n";
    std::cout << "Gold: " << playerGold_ << " | Cost per item: " << itemCost_ << "\n";
    std::cout << "Available modifiers:\n";
    ModifierFactory::listAvailable();
    std::cout << "  [skip] Leave shop\n";
    std::cout << "==========================\n";
}

std::shared_ptr<IModifier> ShopSystem::buyModifier(const std::string& type) {
    if (type == "skip") {
        std::cout << "  Skipped shop.\n";
        return nullptr;
    }

    if (playerGold_ < itemCost_) {
        std::cout << "  Not enough gold!\n";
        return nullptr;
    }

    try {
        auto modifier = ModifierFactory::create(type);
        playerGold_ -= itemCost_;
        std::cout << "  Bought: " << modifier->getName()
                  << " | Gold remaining: " << playerGold_ << "\n";
        return modifier;
    }
    catch (...) {
        std::cout << "  Invalid modifier type. Try again.\n";
        return nullptr;
    }
}

void ShopSystem::refresh() {
    playerGold_ += 2; // earn gold each round
    std::cout << "  Shop refreshed! Gold: " << playerGold_ << "\n";
}
