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

bool enemy_can_enter(const State& s, Pos p) {
    return s.in_bounds(p) && s.tile_at(p) == Tile::Floor && !(p == s.player);
}

void enemy_try_move(State& s, Pos d) {
    const Pos target{s.enemy.x + d.x, s.enemy.y + d.y};
    if (enemy_can_enter(s, target)) s.enemy = target;
}

int sign(int v) { return (v > 0) - (v < 0); }

// Step along the axis with the larger distance (x on ties); if that is blocked, try the
// other axis.
void enemy_chase(State& s) {
    const int dx = s.player.x - s.enemy.x;
    const int dy = s.player.y - s.enemy.y;
    const Pos along_x{sign(dx), 0};
    const Pos along_y{0, sign(dy)};
    const bool x_first = std::abs(dx) >= std::abs(dy);
    const Pos first = x_first ? along_x : along_y;
    const Pos second = x_first ? along_y : along_x;

    const Pos before = s.enemy;
    enemy_try_move(s, first);
    if (s.enemy == before && (second.x != 0 || second.y != 0)) enemy_try_move(s, second);
}

void enemy_wander(State& s) {
    static constexpr Pos kDirs[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    enemy_try_move(s, kDirs[range(s.rng_state, 4)]);
}

void enemy_act(State& s) {
    if (!s.enemy_alive()) return;
    if (adjacent(s.enemy, s.player)) {
        s.player_hp -= kEnemyDamage;
    } else if (range(s.rng_state, kEnemyWanderOdds) == 0) {
        enemy_wander(s);
    } else {
        enemy_chase(s);
    }
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
