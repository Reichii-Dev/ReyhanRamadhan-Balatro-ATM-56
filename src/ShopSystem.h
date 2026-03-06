#pragma once
#include <vector>
#include <memory>
#include "modifiers/IModifier.h"
#include "ModifierFactory.h"

// ============================================================
//  ShopSystem — Lets player buy modifiers between rounds
//  Uses ModifierFactory to create items for sale
// ============================================================
class ShopSystem {
public:
    ShopSystem();

    // Display available items in shop
    void displayShop() const;

    // Player buys a modifier by type string
    // Returns the modifier if purchase succeeds, nullptr if cancelled
    std::shared_ptr<IModifier> buyModifier(const std::string& type);

    // Refresh shop items for next round
    void refresh();

private:
    int playerGold_;
    int itemCost_;
};
