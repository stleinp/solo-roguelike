#include "sim/step.hpp"

#include <cstdlib>

namespace sim {

namespace {

Pos delta(Action a) {
    switch (a) {
        case Action::Up:
            return {0, -1};
        case Action::Down:
            return {0, 1};
        case Action::Left:
            return {-1, 0};
        case Action::Right:
            return {1, 0};
        case Action::Wait:
            break;
    }
    return {0, 0};
}

bool adjacent(Pos a, Pos b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y) == 1; }

void player_act(State& s, Action a) {
    const Pos d = delta(a);
    const Pos target{s.player.x + d.x, s.player.y + d.y};
    if (a == Action::Wait || !s.in_bounds(target)) return;

    if (s.enemy_alive() && target == s.enemy) {
        s.enemy_hp -= kPlayerDamage;
    } else if (s.tile_at(target) != Tile::Wall) {
        s.player = target;
    }
}

void enemy_act(State& s) {
    if (s.enemy_alive() && adjacent(s.enemy, s.player)) s.player_hp -= kEnemyDamage;
}

}  // namespace

Outcome outcome_of(const State& s) {
    if (s.player_hp <= 0) return Outcome::Lost;
    if (s.tile_at(s.player) == Tile::Goal) return Outcome::Won;
    if (s.turn >= kMaxTurns) return Outcome::Timeout;
    return Outcome::Ongoing;
}

StepResult step(State& s, Action a) {
    if (outcome_of(s) != Outcome::Ongoing) return {outcome_of(s)};

    player_act(s, a);
    if (outcome_of(s) == Outcome::Ongoing) enemy_act(s);
    ++s.turn;
    return {outcome_of(s)};
}

}  // namespace sim
