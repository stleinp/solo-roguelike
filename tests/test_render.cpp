#include <doctest/doctest.h>

#include "game/render.hpp"
#include "sim/step.hpp"

using namespace sim;

namespace {
bool contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}
}  // namespace

TEST_CASE("render draws the map with player, enemy, goal and walls") {
    State s = *parse_map("#####\n#@.E>\n#####\n", 1);
    const std::string out = game::render(s, 1, false);
    CHECK(contains(out, "#####\n#@.E>\n#####\n"));
}

TEST_CASE("render shows status and seed") {
    State s = *parse_map("@.E>", 42);
    const std::string out = game::render(s, 42, false);
    CHECK(contains(out, "HP 10/10"));
    CHECK(contains(out, "Enemy 3/3"));
    CHECK(contains(out, "Turn 0/200"));
    CHECK(contains(out, "Seed 42"));
}

TEST_CASE("a dead enemy is not drawn") {
    State s = *parse_map("@E.>", 1);
    s.enemy_hp = 0;
    const std::string out = game::render(s, 1, false);
    CHECK(contains(out, "@..>"));
    CHECK(contains(out, "Enemy dead"));
}

TEST_CASE("render reports the end of the episode") {
    State s = *parse_map("@>.E", 1);
    step(s, Action::Right);
    CHECK(contains(game::render(s, 1, false), "reached the goal"));
}

TEST_CASE("plain output has no escape codes, coloured output does") {
    State s = *parse_map("@.E>", 1);
    CHECK_FALSE(contains(game::render(s, 1, false), "\x1b"));
    CHECK(contains(game::render(s, 1, true), "\x1b["));
}
