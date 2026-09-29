#pragma once

#include <cstdint>

namespace sim {

// Bump-to-attack (B18): moving into the enemy is an attack, so there is no Attack action.
enum class Action : std::uint8_t { Up, Down, Left, Right, Wait };
inline constexpr int kActionCount = 5;

enum class Tile : std::uint8_t { Floor, Wall, Goal };

struct Pos {
    int x = 0;
    int y = 0;
    friend bool operator==(const Pos&, const Pos&) = default;
};

}  // namespace sim
