#pragma once
#include <string>

// ============================================================
//  IModifier — Interface for all score modifiers
//  Pattern: Decorator (Structural)
//  Each modifier wraps a score and transforms it.
// ============================================================
class IModifier {
public:
    virtual ~IModifier() = default;

    // Apply this modifier to a given score and return new score
    virtual int apply(int currentScore) const = 0;

    // Human-readable name for display
    virtual std::string getName() const = 0;

    // Description of what this modifier does
    virtual std::string getDescription() const = 0;
};
