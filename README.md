# Card Run Game — Balatro-Inspired C++ Prototype

A text-based card-run game prototype built in C++ demonstrating Creational, Structural, and Behavioral design patterns.

---

## Architecture Overview

| Class | Role |
|---|---|
| `RunSession` | Main game loop — orchestrates rounds, shop, and scoring |
| `ScoringSystem` | Calculates score using a pluggable Strategy |
| `ShopSystem` | Manages player gold and modifier purchases |
| `ModifierFactory` | Creates modifier objects by type name (Factory pattern) |
| `IModifier` | Interface all modifiers implement |
| `DoubleScoreModifier` | Multiplies score by 2 |
| `FlatBonusModifier` | Adds +50 flat points |
| `SquareScoreModifier` | Squares the score |

---

## Pattern Usage

**Factory (Creational):** `ModifierFactory::create(type)` centralizes all modifier instantiation. `ShopSystem` never calls `new` directly.

**Decorator-like Modifier Chain (Structural):** Each `IModifier::apply(score)` wraps and transforms the score. Applied sequentially in `ScoringSystem::calculate()`.

**Strategy (Behavioral):** `IScoringStrategy` is implemented by `SumScoringStrategy` and `HighCardScoringStrategy`. `ScoringSystem` uses whichever is set, independently of modifier logic.

---

## Modification Log

`SquareScoreModifier` was added to demonstrate extensibility:
1. Created `src/modifiers/SquareScoreModifier.h` implementing `IModifier`
2. Added `"square"` case to `ModifierFactory::create()` in `ModifierFactory.cpp`
3. Added display entry in `ModifierFactory::listAvailable()`

No other files were modified — demonstrating Open/Closed Principle.

---

## How to Build & Run

Open in Visual Studio 2022:
1. Create a new **Console Application (C++)** project
2. Add all files from `src/` into the project
3. Set `main.cpp` as entry point
4. Build and run (`Ctrl+F5`)

---

## AI Usage Disclosure

AI (Claude by Anthropic) was used as a learning assistant to:
- Explain design pattern concepts and how they apply to game architecture
- Guide the structure and naming of classes
- Review code for correctness and pattern compliance

All code was reviewed, understood, and typed/adapted by the student. The student can explain every class, method, and design decision in this project.
