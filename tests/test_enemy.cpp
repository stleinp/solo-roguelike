#include <doctest/doctest.h>

#include <cstdlib>

#include "sim/step.hpp"

using namespace sim;

namespace {
State make(std::string_view map, std::uint64_t seed) {
    auto s = parse_map(map, seed);
    REQUIRE(s.has_value());
    return *s;
}

Action pick(std::uint64_t i) { return static_cast<Action>(i % kActionCount); }
}  // namespace

TEST_CASE("enemy in a corridor closes in on the player") {
    for (std::uint64_t seed = 0; seed < 50; ++seed) {
        State s = make("@.....E>", seed);
        StepResult r;
        for (int i = 0; i < 40 && !r.done(); ++i) r = step(s, Action::Wait);
        const int dist = std::abs(s.enemy.x - s.player.x) + std::abs(s.enemy.y - s.player.y);
        CHECK((r.done() || dist == 1));
    }
}

TEST_CASE("enemy never enters a wall, the goal or the player's tile") {
    const char* map =
        "#######\n"
        "#@..#.#\n"
        "#.#.#>#\n"
        "#.#...#\n"
        "#...E.#\n"
        "#######\n";
    for (std::uint64_t seed = 0; seed < 50; ++seed) {
        State s = make(map, seed);
        for (int i = 0; i < 100; ++i) {
            if (step(s, pick(seed + static_cast<std::uint64_t>(i) * 3)).done()) break;
            CHECK(s.tile_at(s.enemy) == Tile::Floor);
            CHECK_FALSE(s.enemy == s.player);
        }
    }
}

TEST_CASE("a walled-in enemy stays put") {
    State s = make("@.>#\n###E\n", 5);
    for (int i = 0; i < 10; ++i) step(s, Action::Wait);
    CHECK(s.enemy == Pos{3, 1});
}

TEST_CASE("same seed and actions give the same game") {
    const char* map = "@......\n.......\n....E.>\n";
    State a = make(map, 99);
    State b = make(map, 99);
    for (int i = 0; i < 60; ++i) {
        const Action act = pick(static_cast<std::uint64_t>(i) * 7);
        step(a, act);
        step(b, act);
        CHECK(a.player == b.player);
        CHECK(a.enemy == b.enemy);
        CHECK(a.rng_state == b.rng_state);
    }
}

TEST_CASE("different seeds lead to different enemy paths") {
    const char* map = "@..........\n...........\n.....E.....\n..........>\n";
    bool differs = false;
    State a = make(map, 1);
    State b = make(map, 2);
    for (int i = 0; i < 20; ++i) {
        step(a, Action::Wait);
        step(b, Action::Wait);
        if (!(a.enemy == b.enemy)) differs = true;
    }
    CHECK(differs);
}
