#include "game/layout.hpp"

namespace game {

int sprite_index(Sprite s) {
    switch (s) {
        case Sprite::Floor:
            return 0;
        case Sprite::Wall:
            return 40;
        case Sprite::Goal:
            return 45;
        case Sprite::Player:
            return 96;
        case Sprite::Enemy:
            return 110;
    }
    return 0;
}

Rect sheet_rect(int index) {
    return {(index % kSheetColumns) * kSheetTilePx, (index / kSheetColumns) * kSheetTilePx,
            kSheetTilePx, kSheetTilePx};
}

Rect cell_rect(int x, int y, int tile_px) { return {x * tile_px, y * tile_px, tile_px, tile_px}; }

Size window_size(int width, int height, int tile_px, int hud_px) {
    return {width * tile_px, height * tile_px + hud_px};
}

}  // namespace game
