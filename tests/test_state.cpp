#include <doctest/doctest.h>

#include "sim/state.hpp"

using namespace sim;

TEST_CASE("parse_map reads a valid map") {
    auto s = parse_map(
        "#####\n"
        "#@..#\n"
        "#.E>#\n"
        "#####\n");
    REQUIRE(s.has_value());
    CHECK(s->width == 5);
    CHECK(s->height == 4);
    CHECK(s->tiles.size() == 20);
    CHECK(s->player == Pos{1, 1});
    CHECK(s->enemy == Pos{2, 2});
    CHECK(s->tile_at({0, 0}) == Tile::Wall);
    CHECK(s->tile_at({3, 2}) == Tile::Goal);
    CHECK(s->tile_at({1, 1}) == Tile::Floor);  // start tiles are floor
    CHECK(s->tile_at({2, 2}) == Tile::Floor);
}

TEST_CASE("parse_map sets initial values") {
    auto s = parse_map("@E>");
    REQUIRE(s.has_value());
    CHECK(s->player_hp == kPlayerStartHp);
    CHECK(s->enemy_hp == kEnemyStartHp);
    CHECK(s->turn == 0);
}

TEST_CASE("parse_map accepts a missing trailing newline") {
    CHECK(parse_map("@E>").has_value());
    CHECK(parse_map("@E>\n").has_value());
}

TEST_CASE("parse_map rejects invalid maps") {
    CHECK_FALSE(parse_map("").has_value());
    CHECK_FALSE(parse_map("@E").has_value());          // no goal
    CHECK_FALSE(parse_map("E>.").has_value());         // no player
    CHECK_FALSE(parse_map("@>.").has_value());         // no enemy
    CHECK_FALSE(parse_map("@@E>").has_value());        // two players
    CHECK_FALSE(parse_map("@E>E").has_value());        // two enemies
    CHECK_FALSE(parse_map("@E>\n..").has_value());     // ragged rows
    CHECK_FALSE(parse_map("@E>\n\n...").has_value());  // empty row
    CHECK_FALSE(parse_map("@E>?").has_value());        // unknown character
}

TEST_CASE("in_bounds") {
    auto s = parse_map("@E>");
    REQUIRE(s.has_value());
    CHECK(s->in_bounds({0, 0}));
    CHECK(s->in_bounds({2, 0}));
    CHECK_FALSE(s->in_bounds({3, 0}));
    CHECK_FALSE(s->in_bounds({-1, 0}));
    CHECK_FALSE(s->in_bounds({0, 1}));
}
