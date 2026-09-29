#pragma once

#include "sim/state.hpp"

// Turn animation for the windowed frontend (B23). Pure functions, no raylib. The State always
// holds the result of the turn at once; the animation is only how the view gets there.
// Floats are allowed here (never in sim, B14).

namespace game {

inline constexpr float kTurnSeconds = 0.12f;
inline constexpr float kFlashSeconds = 0.2f;

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

// What one turn changed, as far as the view needs to know: where the actors came from and how
// much damage each took.
struct TurnDelta {
    sim::Pos player_from;
    sim::Pos enemy_from;
    bool enemy_was_alive = false;
    int player_damage = 0;
    int enemy_damage = 0;
};

TurnDelta make_delta(const sim::State& before, const sim::State& after);

// Fast start, soft landing. t is clamped to [0, 1]; ease_out(0) = 0 and ease_out(1) = 1.
float ease_out(float t);

// Cell position at animation progress t in [0, 1]: from at t = 0, to at t = 1.
Vec2 lerp_cell(sim::Pos from, sim::Pos to, float t);

// Strength of the hit flash, 1 at the moment of the hit fading to 0 after kFlashSeconds.
float flash_strength(float seconds_since_hit);

}  // namespace game
