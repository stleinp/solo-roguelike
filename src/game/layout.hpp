#pragma once

// Pure geometry for the windowed frontend: where a map cell goes on screen and where a sprite
// sits in the tilesheet. No raylib in here, so it is testable without a window (B23).

namespace game {

struct Rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    friend bool operator==(const Rect&, const Rect&) = default;
};

struct Size {
    int w = 0;
    int h = 0;
    friend bool operator==(const Size&, const Size&) = default;
};

// The tilesheet is packed: 16x16 tiles, no spacing, 12 columns.
inline constexpr int kSheetTilePx = 16;
inline constexpr int kSheetColumns = 12;

enum class Sprite { Floor, Wall, Goal, Player, Enemy };

// Index of the sprite in the tilesheet, counted row by row from the top left.
int sprite_index(Sprite s);

// Source rectangle of a tilesheet index, in sheet pixels.
Rect sheet_rect(int index);

// Destination rectangle of map cell (x, y) when every tile is drawn `tile_px` pixels wide.
Rect cell_rect(int x, int y, int tile_px);

// Window size for a map of width x height cells plus a HUD strip below it.
Size window_size(int width, int height, int tile_px, int hud_px);

}  // namespace game
