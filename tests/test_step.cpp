#include <doctest/doctest.h>

#include "sim/step.hpp"

using namespace sim;

namespace {
State make(std::string_view map) {
    auto s = parse_map(map);
    REQUIRE(s.has_value());
    return *s;
}
}  // namespace

TEST_CASE("moving on open floor") {
    State s = make("@..>E");
    auto r = step(s, Action::Right);
    CHECK(s.player == Pos{1, 0});
    CHECK(s.turn == 1);
    CHECK(r.outcome == Outcome::Ongoing);
    CHECK_FALSE(r.done());
}

TEST_CASE("all four directions") {
    State s = make(
        ".....\n"
        ".....\n"
        "..@>.\n"
        ".....\n"
        "E....\n");
    step(s, Action::Up);
    CHECK(s.player == Pos{2, 1});
    step(s, Action::Left);
    CHECK(s.player == Pos{1, 1});
    step(s, Action::Down);
    CHECK(s.player == Pos{1, 2});
    step(s, Action::Right);
    CHECK(s.player == Pos{2, 2});
}

TEST_CASE("wait keeps position but spends the turn") {
    State s = make("@.>E");
    step(s, Action::Wait);
    CHECK(s.player == Pos{0, 0});
    CHECK(s.turn == 1);
}

TEST_CASE("walls and grid edges block movement") {
    State s = make(
        "@#>\n"
        "..E\n");
    step(s, Action::Right);  // wall
    CHECK(s.player == Pos{0, 0});
    step(s, Action::Up);  // off the grid
    CHECK(s.player == Pos{0, 0});
    step(s, Action::Left);  // off the grid
    CHECK(s.player == Pos{0, 0});
    CHECK(s.turn == 3);  // blocked moves still cost a turn
}

TEST_CASE("bump-to-attack damages the enemy and does not move the player") {
    State s = make("@E.>");
    step(s, Action::Right);
    CHECK(s.player == Pos{0, 0});
    CHECK(s.enemy_hp == kEnemyStartHp - kPlayerDamage);
}

TEST_CASE("a killed enemy stops existing and its tile becomes walkable") {
    State s = make("@E.>");
    s.enemy_hp = 1;
    step(s, Action::Right);
    CHECK_FALSE(s.enemy_alive());
    step(s, Action::Right);
    CHECK(s.player == Pos{1, 0});
}

TEST_CASE("an adjacent enemy hits the player after the player's action") {
    State s = make("@E.>");
    step(s, Action::Wait);
    CHECK(s.player_hp == kPlayerStartHp - kEnemyDamage);
}

TEST_CASE("a distant enemy does not hit") {
    State s = make("@..>E");
    step(s, Action::Wait);
    CHECK(s.player_hp == kPlayerStartHp);
}

TEST_CASE("a dead enemy does not hit") {
    State s = make("@E.>");
    s.enemy_hp = 1;
    step(s, Action::Right);  // kills it
    CHECK(s.player_hp == kPlayerStartHp);
}

TEST_CASE("reaching the goal wins, and the enemy does not act afterwards") {
    State s = make(
        "@>E\n"
        "...\n");
    auto r = step(s, Action::Right);
    CHECK(r.outcome == Outcome::Won);
    CHECK(r.done());
    CHECK(s.player_hp == kPlayerStartHp);
}

TEST_CASE("player at zero hp loses") {
    State s = make("@E.>");
    s.player_hp = kEnemyDamage;
    auto r = step(s, Action::Wait);
    CHECK(r.outcome == Outcome::Lost);
    CHECK(s.player_hp <= 0);
}

TEST_CASE("timeout after kMaxTurns") {
    State s = make("@..>E");
    s.turn = kMaxTurns - 1;
    CHECK(step(s, Action::Wait).outcome == Outcome::Timeout);
}

TEST_CASE("stepping a finished episode changes nothing") {
    State s = make("@>E");
    step(s, Action::Right);
    const State before = s;
    auto r = step(s, Action::Left);
    CHECK(r.outcome == Outcome::Won);
    CHECK(s.player == before.player);
    CHECK(s.turn == before.turn);
}
