#pragma once

#include <chrono>
#include <cstdint>

namespace game {

// The one fixed level of the game, shared by all frontends.
inline constexpr const char* kLevel =
    "###########\n"
    "#@....#...#\n"
    "#.###.#.#.#\n"
    "#.#...#.#.#\n"
    "#.#.###.#.#\n"
    "#...E...#>#\n"
    "###########\n";

// Unlike sim, the game may read the clock: it only picks the seed. The seed is shown on
// screen, and `delve <seed>` replays the same game.
inline std::uint64_t clock_seed() {
    return static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
}

}  // namespace game
