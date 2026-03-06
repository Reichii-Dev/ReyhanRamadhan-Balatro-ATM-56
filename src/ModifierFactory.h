#pragma once
#include <memory>
#include <string>
#include "modifiers/IModifier.h"

// ============================================================
//  ModifierFactory — Creates modifier objects by name
//  Pattern: Factory (Creational)
//  Centralizes creation logic so client code never calls `new` directly.
// ============================================================
class ModifierFactory {
public:
    // Create a modifier by type name
    // Valid types: "double", "flat", "square"
    static std::shared_ptr<IModifier> create(const std::string& type);

    // List all available modifier types
    static void listAvailable();
};
