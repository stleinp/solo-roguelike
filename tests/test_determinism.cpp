#include <doctest/doctest.h>

#include "sim/hash.hpp"
#include "sim/step.hpp"

using namespace sim;

namespace {

constexpr const char* kMap =
    "###########\n"
    "#@....#...#\n"
    "#.###.#.#.#\n"
    "#.#...#.#.#\n"
    "#.#.###.#.#\n"
    "#...E...#>#\n"
    "###########\n";

// Plays `steps` actions drawn from a separate RNG stream (so the actions do not depend
// on the game's own RNG) and returns the hash of the final state.
std::uint64_t play(std::uint64_t game_seed, std::uint64_t action_seed, int steps) {
    auto s = parse_map(kMap, game_seed);
    REQUIRE(s.has_value());
    RngState actions = make_rng(action_seed);
    for (int i = 0; i < steps; ++i) {
        step(*s, static_cast<Action>(range(actions, kActionCount)));
    }
    return hash(*s);
}

}  // namespace

TEST_CASE("same seed and actions give the same final hash") {
    for (std::uint64_t seed = 0; seed < 100; ++seed) {
        CHECK(play(seed, seed + 1000, 150) == play(seed, seed + 1000, 150));
    }
}

TEST_CASE("different game seeds give different final hashes") {
    CHECK(play(1, 5, 150) != play(2, 5, 150));
}

TEST_CASE("different action sequences give different final hashes") {
    CHECK(play(1, 5, 150) != play(1, 6, 150));
}

TEST_CASE("hash reacts to every part of the state") {
    State base = *parse_map(kMap, 1);
    const std::uint64_t h = hash(base);

    State a = base;
    a.player.x += 1;
    CHECK(hash(a) != h);
    a = base;
    a.enemy.y += 1;
    CHECK(hash(a) != h);
    a = base;
    a.player_hp -= 1;
    CHECK(hash(a) != h);
    a = base;
    a.enemy_hp -= 1;
    CHECK(hash(a) != h);
    a = base;
    a.turn += 1;
    CHECK(hash(a) != h);
    a = base;
    a.rng_state[3] ^= 1;
    CHECK(hash(a) != h);
    a = base;
    a.tiles[0] = Tile::Floor;
    CHECK(hash(a) != h);
}

// Golden value: changes only deliberately (any rule, RNG or hash change alters it).
// When it does, update it and note why in docs/devlog.md.
TEST_CASE("golden hash") { CHECK(play(12345, 67890, 200) == 0x19936cd070a5f202ULL); }
