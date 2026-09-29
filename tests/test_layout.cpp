#include <doctest/doctest.h>

#include <initializer_list>

#include "game/layout.hpp"

using namespace game;

TEST_CASE("sheet_rect maps an index to sheet pixels, row by row") {
    CHECK(sheet_rect(0) == Rect{0, 0, 16, 16});
    CHECK(sheet_rect(11) == Rect{176, 0, 16, 16});  // last column of the first row
    CHECK(sheet_rect(12) == Rect{0, 16, 16, 16});   // first column of the second row
    CHECK(sheet_rect(96) == Rect{0, 128, 16, 16});
    CHECK(sheet_rect(131) == Rect{176, 160, 16, 16});  // the last tile of the 12x11 sheet
}

TEST_CASE("every sprite lies inside the 12x11 sheet") {
    for (Sprite s : {Sprite::Floor, Sprite::Wall, Sprite::Goal, Sprite::Player, Sprite::Enemy}) {
        const int i = sprite_index(s);
        CHECK(i >= 0);
        CHECK(i < 12 * 11);
    }
}

TEST_CASE("sprites use distinct tiles") {
    CHECK(sprite_index(Sprite::Floor) != sprite_index(Sprite::Wall));
    CHECK(sprite_index(Sprite::Player) != sprite_index(Sprite::Enemy));
    CHECK(sprite_index(Sprite::Goal) != sprite_index(Sprite::Floor));
}

TEST_CASE("cell_rect scales cells to the tile size") {
    CHECK(cell_rect(0, 0, 64) == Rect{0, 0, 64, 64});
    CHECK(cell_rect(3, 2, 64) == Rect{192, 128, 64, 64});
    CHECK(cell_rect(3, 2, 32) == Rect{96, 64, 32, 32});
}

TEST_CASE("window_size adds the HUD strip below the map") {
    CHECK(window_size(11, 7, 64, 60) == Size{704, 508});
}
