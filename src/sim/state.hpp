#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

#include "sim/types.hpp"

namespace sim {

inline constexpr int kPlayerStartHp = 10;
inline constexpr int kEnemyStartHp = 3;

// The complete game state: plain data, copyable, no pointers (B13).
// Copying a State copies the RNG state too, so a copy replays identically.
struct State {
    int width = 0;
    int height = 0;
    std::vector<Tile> tiles;  // row-major, size width * height

    Pos player;
    int player_hp = kPlayerStartHp;
    Pos enemy;
    int enemy_hp = kEnemyStartHp;

    int turn = 0;
    std::array<std::uint64_t, 4> rng_state{};  // filled in by the RNG in M3

    Tile tile_at(Pos p) const { return tiles[static_cast<std::size_t>(p.y * width + p.x)]; }
    bool in_bounds(Pos p) const { return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height; }
};

// Parses a map from text, one row per line:
//   '#' wall   '.' floor   '>' goal   '@' player start   'E' enemy start
// Player and enemy start tiles become floor. Requires rectangular rows, exactly one '@',
// exactly one 'E' and at least one '>'. Returns nullopt otherwise.
std::optional<State> parse_map(std::string_view text);

}  // namespace sim
