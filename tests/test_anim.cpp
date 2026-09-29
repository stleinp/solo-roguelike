#include <doctest/doctest.h>

#include "game/anim.hpp"
#include "sim/step.hpp"

using namespace game;

namespace {

sim::State make_state() {
    auto s = sim::parse_map(
        "#####\n"
        "#@.E>\n"
        "#####\n",
        1);
    REQUIRE(s.has_value());
    return *s;
}

}  // namespace

TEST_CASE("ease_out runs from 0 to 1 and is clamped") {
    CHECK(ease_out(0.0f) == doctest::Approx(0.0f));
    CHECK(ease_out(1.0f) == doctest::Approx(1.0f));
    CHECK(ease_out(-1.0f) == doctest::Approx(0.0f));
    CHECK(ease_out(2.0f) == doctest::Approx(1.0f));
    CHECK(ease_out(0.5f) > 0.5f);  // fast start
}

TEST_CASE("lerp_cell starts at from and ends at to") {
    const Vec2 a = lerp_cell({1, 2}, {2, 2}, 0.0f);
    CHECK(a.x == doctest::Approx(1.0f));
    CHECK(a.y == doctest::Approx(2.0f));
    const Vec2 b = lerp_cell({1, 2}, {2, 2}, 1.0f);
    CHECK(b.x == doctest::Approx(2.0f));
    CHECK(b.y == doctest::Approx(2.0f));
    const Vec2 mid = lerp_cell({1, 2}, {1, 3}, 0.5f);
    CHECK(mid.x == doctest::Approx(1.0f));
    CHECK(mid.y > 2.0f);
    CHECK(mid.y < 3.0f);
}

TEST_CASE("lerp_cell of an actor that did not move stays put") {
    const Vec2 v = lerp_cell({3, 1}, {3, 1}, 0.4f);
    CHECK(v.x == doctest::Approx(3.0f));
    CHECK(v.y == doctest::Approx(1.0f));
}

TEST_CASE("flash_strength fades from 1 to 0") {
    CHECK(flash_strength(0.0f) == doctest::Approx(1.0f));
    CHECK(flash_strength(kFlashSeconds / 2) == doctest::Approx(0.5f));
    CHECK(flash_strength(kFlashSeconds) == doctest::Approx(0.0f));
    CHECK(flash_strength(10.0f) == doctest::Approx(0.0f));
}

TEST_CASE("make_delta reports a move without damage") {
    sim::State s = make_state();
    const sim::State before = s;
    sim::step(s, sim::Action::Wait);
    const TurnDelta d = make_delta(before, s);
    CHECK(d.player_from == before.player);
    CHECK(d.enemy_from == before.enemy);
    CHECK(d.enemy_was_alive);
    CHECK(d.enemy_damage == 0);
}

TEST_CASE("make_delta reports damage dealt by bump-to-attack") {
    // Known situation: the player stands next to the enemy and attacks.
    sim::State s = make_state();
    s.player = {2, 1};
    s.enemy = {3, 1};
    const sim::State before = s;
    sim::step(s, sim::Action::Right);
    const TurnDelta d = make_delta(before, s);
    CHECK(d.enemy_damage == sim::kPlayerDamage);
    CHECK(d.player_from == sim::Pos{2, 1});
    CHECK(d.enemy_was_alive);
}
