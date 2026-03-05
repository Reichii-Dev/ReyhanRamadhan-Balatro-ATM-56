# Design Pattern Analysis

## PART A — Creational Pattern Analysis

### A1 — Factory

Modifier objects are created in `ModifierFactory.cpp` inside the static method `ModifierFactory::create(const std::string& type)`.

The `ModifierFactory` class is responsible for all modifier instantiation. When `ShopSystem::buyModifier()` is called, it delegates to `ModifierFactory::create()` instead of using `new` directly.

**Why centralizing creation is beneficial:**
- Client code (`ShopSystem`, `RunSession`) never needs to know about concrete modifier classes.
- Adding a new modifier only requires editing `ModifierFactory.cpp` — no changes to shop or game loop logic.
- Reduces coupling between systems.

Reference: `src/ModifierFactory.h`, `src/ModifierFactory.cpp`, function `ModifierFactory::create()`

---

### A2 — Extensibility

To add `SquareScoreModifier`, the following steps were taken:

1. Created `src/modifiers/SquareScoreModifier.h` — new class implementing `IModifier`
2. Added `"square"` case in `ModifierFactory::create()` in `ModifierFactory.cpp`
3. Added display entry in `ModifierFactory::listAvailable()`

**Files changed:** `SquareScoreModifier.h` (new), `ModifierFactory.cpp` (2 lines added)

**Was the design easy to extend?** Yes. Because `IModifier` defines a clean interface (`apply`, `getName`, `getDescription`), and all creation is centralized in `ModifierFactory`, adding a new modifier required zero changes to `RunSession`, `ScoringSystem`, or `ShopSystem`. This demonstrates the Open/Closed Principle.

---

### A3 — Factory Method (Conceptual)

If the game had multiple shop types (e.g., `WeaponShop`, `ModifierShop`, `CardShop`), a Factory Method pattern would be appropriate.

Example interface:
```cpp
class IShopFactory {
public:
    virtual std::shared_ptr<IShop> createShop() = 0;
};

class ModifierShopFactory : public IShopFactory {
public:
    std::shared_ptr<IShop> createShop() override {
        return std::make_shared<ModifierShop>();
    }
};
```

Each factory subclass decides which shop type to instantiate. `RunSession` would only depend on `IShopFactory`, not concrete shop classes.

---

### A4 — Prototype (Conceptual)

`Clone()` would be useful when a player wants to duplicate an existing modifier (e.g., equip a second copy of `DoubleScoreModifier` without going back to the shop).

Implementation location:
```cpp
class IModifier {
public:
    virtual std::shared_ptr<IModifier> clone() const = 0;
};

class DoubleScoreModifier : public IModifier {
public:
    std::shared_ptr<IModifier> clone() const override {
        return std::make_shared<DoubleScoreModifier>(*this);
    }
};
```

`Clone()` would be called in `RunSession` when a "duplicate" item is purchased from the shop.

---

### A5 — Singleton (Conceptual)

In this project, no global singleton objects were used. `RunSession` owns `ScoringSystem` and `ShopSystem` directly as members.

**Advantages of Singleton:** Ensures a single instance of a shared resource (e.g., a global `AudioManager` or `Logger`).

**Risks:**
- Hidden global state makes testing harder.
- Tight coupling — any class can access and modify the singleton.
- Not thread-safe without explicit locking.

For this game's scale, direct ownership inside `RunSession` is cleaner than a Singleton.

---

## PART B — Structural Pattern Analysis

### B1 — Decorator Concept

The modifier system behaves like a Decorator pattern. The "base object" being decorated is the **integer score** returned by `IScoringStrategy::calculateBase()`.

Each `IModifier::apply(int score)` takes a score and returns a new score — wrapping and transforming it, just like a Decorator wraps an object and extends its behavior.

In `ScoringSystem::calculate()` (`src/ScoringSystem.cpp`):
```cpp
int score = strategy_->calculateBase(handValues);
for (const auto& mod : modifiers) {
    score = mod->apply(score);  // Each modifier wraps/transforms score
}
```

The chain: `Base Score → DoubleScoreModifier → FlatBonusModifier → Final Score`

---

### B2 — Modifier Order

Yes, modifier order affects the final score significantly.

Example using code values:
- Base score: `30`
- Apply `DoubleScoreModifier` first: `30 × 2 = 60`
- Apply `FlatBonusModifier(50)` second: `60 + 50 = 110`

Reversed order:
- Apply `FlatBonusModifier(50)` first: `30 + 50 = 80`
- Apply `DoubleScoreModifier` second: `80 × 2 = 160`

The second order gives 160 vs 110 — a 45% difference. Order matters and is maintained by the `std::vector<std::shared_ptr<IModifier>> activeModifiers_` in `RunSession`.

---

### B3 — Composite (Conceptual)

A modifier group (composite) would allow treating a set of modifiers as a single modifier:

```cpp
class ModifierGroup : public IModifier {
    std::vector<std::shared_ptr<IModifier>> children_;
public:
    void add(std::shared_ptr<IModifier> m) { children_.push_back(m); }
    int apply(int score) const override {
        for (auto& m : children_) score = m->apply(score);
        return score;
    }
};
```

This would let the shop sell "modifier bundles" as one purchasable item.

---

### B4 — Adapter (Conceptual)

If an external scoring library used a different interface, e.g.:
```cpp
class ExternalScorer {
public:
    float computeScore(std::vector<float> values);
};
```

An Adapter would make it compatible with `IScoringStrategy`:
```cpp
class ExternalScorerAdapter : public IScoringStrategy {
    ExternalScorer external_;
public:
    int calculateBase(const std::vector<int>& handValues) const override {
        std::vector<float> floats(handValues.begin(), handValues.end());
        return static_cast<int>(external_.computeScore(floats));
    }
};
```

No changes to `ScoringSystem` or `RunSession` would be needed.

---

### B5 — Facade (Conceptual)

A `GameFacade` would expose a simplified interface over all subsystems:

```cpp
class GameFacade {
public:
    void startGame();
    int playHand(std::vector<int> hand);
    void openShop();
    void buyModifier(const std::string& type);
    bool isRunComplete() const;
};
```

Internally it wraps `RunSession`, `ScoringSystem`, `ShopSystem`, and `ModifierFactory`. A UI layer or test harness would only interact with `GameFacade`, not individual systems.

---

## PART C — Behavioral Pattern Analysis

### C1 — Strategy

Scoring logic exists in `src/ScoringSystem.h` and `src/ScoringSystem.cpp`.

Two concrete strategies are implemented:
- `SumScoringStrategy` — sums all card values
- `HighCardScoringStrategy` — uses only the highest card value

To support more rules, simply implement `IScoringStrategy` with a new class and pass it to `ScoringSystem`:
```cpp
scoringSystem_.setStrategy(std::make_shared<HighCardScoringStrategy>());
```

No changes to `RunSession` are needed. This is the core benefit of Strategy pattern.

---

### C2 — Observer (Conceptual)

Events in the system that could use Observer:
- `onRoundCleared` — notify modifiers that a round was beaten
- `onHandPlayed` — some modifiers might want to react per card played
- `onModifierBought` — UI could update to reflect new modifier chain

Example:
```cpp
class IGameEventListener {
public:
    virtual void onRoundCleared(int round, int score) = 0;
};
```

`RunSession` would maintain a list of listeners and notify them at key events. This decouples game logic from side effects (e.g., logging, UI updates, achievement triggers).

---

### C3 — Command (Conceptual)

Actions that could be Command objects:
- `PlayHandCommand` — plays the current hand and triggers scoring
- `BuyModifierCommand` — purchases a modifier from the shop
- `SkipShopCommand` — ends shop phase without purchase

```cpp
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};
```

Commands enable undo/redo history, action logging, and replay features.

---

### C4 — State (Conceptual)

The run has distinct phases that map naturally to State pattern:

```
PlayingState → ScoringState → ShopState → NextRoundState → GameOverState
```

```cpp
class IGameState {
public:
    virtual void enter(RunSession& session) = 0;
    virtual void update(RunSession& session) = 0;
    virtual void exit(RunSession& session) = 0;
};
```

Currently, these phases are managed implicitly inside `RunSession::playRound()`. Using explicit State objects would make phase transitions clearer, easier to extend (e.g., add a `BossBlindState`), and testable in isolation.
