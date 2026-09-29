#include "game/anim.hpp"

namespace game {

TurnDelta make_delta(const sim::State& before, const sim::State& after) {
    TurnDelta d;
    d.player_from = before.player;
    d.enemy_from = before.enemy;
    d.enemy_was_alive = before.enemy_alive();
    d.player_damage = before.player_hp - after.player_hp;
    d.enemy_damage = before.enemy_hp - after.enemy_hp;
    return d;
}

float ease_out(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float u = 1.0f - t;
    return 1.0f - u * u;
}

Vec2 lerp_cell(sim::Pos from, sim::Pos to, float t) {
    const float e = ease_out(t);
    const auto fx = static_cast<float>(from.x);
    const auto fy = static_cast<float>(from.y);
    return {fx + (static_cast<float>(to.x) - fx) * e, fy + (static_cast<float>(to.y) - fy) * e};
}

float flash_strength(float seconds_since_hit) {
    if (seconds_since_hit <= 0.0f) return 1.0f;
    if (seconds_since_hit >= kFlashSeconds) return 0.0f;
    return 1.0f - seconds_since_hit / kFlashSeconds;
}

}  // namespace game
