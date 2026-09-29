#pragma once

#include "sim/state.hpp"
#include "sim/types.hpp"

namespace sim {

inline constexpr int kPlayerDamage = 1;
inline constexpr int kEnemyDamage = 1;
inline constexpr int kMaxTurns = 200;

enum class Outcome : std::uint8_t { Ongoing, Won, Lost, Timeout };

struct StepResult {
    Outcome outcome = Outcome::Ongoing;
    bool done() const { return outcome != Outcome::Ongoing; }
};

// The outcome follows from the state alone, so State needs no separate "done" flag.
// Precedence: Lost, then Won, then Timeout.
Outcome outcome_of(const State& s);

// Plays one turn: the player acts, then the enemy (if alive) acts, then the turn counter
// advances. Rules:
//   - Moving into a wall or off the grid does nothing, but the turn is spent.
//   - Moving into the living enemy attacks it instead of moving (bump-to-attack).
//   - Stepping on the goal ends the episode at once (Won); the enemy does not act.
//   - An enemy next to the player hits it. (Enemy movement arrives in M3.)
//   - The episode ends on Won, Lost (player hp <= 0) or Timeout (kMaxTurns).
// Calling step on a finished episode changes nothing and returns its outcome.
StepResult step(State& s, Action a);

}  // namespace sim
