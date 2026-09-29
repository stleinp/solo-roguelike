// Windowed frontend (B23, phase 2b). Like the terminal frontend it only reads the State and
// sends actions back. G1: tilesheet sprites, HUD and end screen. G2: turn animation.
//
// The State holds the result of a turn at once. The animation is view-only: TurnDelta says where
// the actors came from and `elapsed` says how far along the slide is. One key press is buffered
// while an animation runs, so fast play is not lost.
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include <raylib.h>

#include "game/anim.hpp"
#include "game/input.hpp"
#include "game/layout.hpp"
#include "game/level.hpp"
#include "sim/step.hpp"

namespace {

constexpr int kTilePx = 64;  // the 16 px sprites are drawn 4x, without smoothing
constexpr int kHudPx = 72;

game::Key poll_key() {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_K)) return game::Key::Up;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_J)) return game::Key::Down;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_H)) return game::Key::Left;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_L)) return game::Key::Right;
    if (IsKeyPressed(KEY_SPACE)) return game::Key::Wait;
    if (IsKeyPressed(KEY_R)) return game::Key::Restart;
    if (IsKeyPressed(KEY_Q)) return game::Key::Quit;
    return game::Key::Other;
}

Rectangle to_raylib(const game::Rect& r) {
    return {static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.w),
            static_cast<float>(r.h)};
}

// What the view is currently showing of the last turn.
struct TurnView {
    bool active = false;
    float elapsed = 0.0f;  // seconds since the turn started
    game::TurnDelta delta;
};

void draw_sprite(Texture2D sheet, game::Sprite sprite, game::Vec2 cell, Color tint = WHITE) {
    const Rectangle dest{cell.x * kTilePx, cell.y * kTilePx, kTilePx, kTilePx};
    DrawTexturePro(sheet, to_raylib(game::sheet_rect(game::sprite_index(sprite))), dest, {0, 0},
                   0.0f, tint);
}

void draw_sprite(Texture2D sheet, game::Sprite sprite, sim::Pos p) {
    draw_sprite(sheet, sprite, game::Vec2{static_cast<float>(p.x), static_cast<float>(p.y)});
}

// White normally, red when hit: the flash strength scales the reduction of green and blue.
Color hit_tint(float strength, unsigned char alpha = 255) {
    const auto reduce = static_cast<unsigned char>(255.0f - 160.0f * strength);
    return Color{255, reduce, reduce, alpha};
}

void draw_bar(int x, int y, int w, int value, int max, Color fill) {
    constexpr int kH = 14;
    DrawRectangle(x, y, w, kH, Color{50, 30, 30, 255});
    if (value > 0) DrawRectangle(x, y, w * value / max, kH, fill);
    DrawRectangleLines(x, y, w, kH, RAYWHITE);
}

void draw_centered(const char* text, int cx, int y, int size, Color c) {
    DrawText(text, cx - MeasureText(text, size) / 2, y, size, c);
}

void draw_hud(const sim::State& s, std::uint64_t seed) {
    const int y = s.height * kTilePx;
    DrawRectangle(0, y, s.width * kTilePx, kHudPx, Color{24, 20, 28, 255});

    DrawText("HP", 10, y + 10, 18, RAYWHITE);
    draw_bar(44, y + 10, 160, s.player_hp, sim::kPlayerStartHp, Color{80, 200, 90, 255});
    const std::string hp = std::to_string(s.player_hp) + "/" + std::to_string(sim::kPlayerStartHp);
    DrawText(hp.c_str(), 212, y + 10, 18, RAYWHITE);

    DrawText("Enemy", 300, y + 10, 18, RAYWHITE);
    if (s.enemy_alive()) {
        draw_bar(372, y + 10, 100, s.enemy_hp, sim::kEnemyStartHp, Color{210, 70, 60, 255});
    } else {
        DrawText("dead", 372, y + 10, 18, GRAY);
    }

    const std::string turn = "Turn " + std::to_string(s.turn) + "/" + std::to_string(sim::kMaxTurns);
    DrawText(turn.c_str(), 10, y + 40, 18, RAYWHITE);
    const std::string seed_text = "Seed " + std::to_string(seed);
    DrawText(seed_text.c_str(), 130, y + 40, 18, LIGHTGRAY);
    DrawText("Move: arrows/wasd/hjkl   Wait: space   Restart: r   Quit: q", 10, y + 56, 12, GRAY);
}

void draw_end_screen(const sim::State& s, sim::Outcome outcome) {
    const char* title = "";
    Color color = WHITE;
    switch (outcome) {
        case sim::Outcome::Won:
            title = "You reached the goal!";
            color = Color{120, 230, 120, 255};
            break;
        case sim::Outcome::Lost:
            title = "You died.";
            color = Color{230, 90, 80, 255};
            break;
        case sim::Outcome::Timeout:
            title = "Out of time.";
            color = Color{230, 200, 90, 255};
            break;
        case sim::Outcome::Ongoing:
            return;
    }
    const int w = s.width * kTilePx;
    const int h = s.height * kTilePx;
    DrawRectangle(0, 0, w, h, Color{0, 0, 0, 170});
    draw_centered(title, w / 2, h / 2 - 40, 40, color);
    draw_centered("[r] new game    [q] quit", w / 2, h / 2 + 16, 20, RAYWHITE);
}

void draw(const sim::State& s, std::uint64_t seed, Texture2D sheet, const TurnView& view) {
    ClearBackground(BLACK);
    for (int y = 0; y < s.height; ++y) {
        for (int x = 0; x < s.width; ++x) {
            const sim::Pos p{x, y};
            // Floor under everything: the character and door sprites have transparent corners.
            draw_sprite(sheet, game::Sprite::Floor, p);
            switch (s.tile_at(p)) {
                case sim::Tile::Wall: draw_sprite(sheet, game::Sprite::Wall, p); break;
                case sim::Tile::Goal: draw_sprite(sheet, game::Sprite::Goal, p); break;
                case sim::Tile::Floor: break;
            }
        }
    }

    // Progress of the slide in [0, 1]; 1 when nothing is animating.
    const float t = view.active ? view.elapsed / game::kTurnSeconds : 1.0f;
    const float since_hit = view.active ? view.elapsed : game::kFlashSeconds;
    const game::TurnDelta& d = view.delta;

    // The enemy stays visible during the turn in which it dies and fades out.
    if (s.enemy_alive() || (view.active && d.enemy_was_alive)) {
        const game::Vec2 pos = view.active ? game::lerp_cell(d.enemy_from, s.enemy, t)
                                           : game::Vec2{static_cast<float>(s.enemy.x),
                                                        static_cast<float>(s.enemy.y)};
        const float flash = (view.active && d.enemy_damage > 0) ? game::flash_strength(since_hit) : 0.0f;
        const auto alpha = static_cast<unsigned char>(s.enemy_alive() ? 255.0f : 255.0f * (1.0f - t));
        draw_sprite(sheet, game::Sprite::Enemy, pos, hit_tint(flash, alpha));
    }

    const game::Vec2 player_pos = view.active ? game::lerp_cell(d.player_from, s.player, t)
                                              : game::Vec2{static_cast<float>(s.player.x),
                                                           static_cast<float>(s.player.y)};
    const float player_flash =
        (view.active && d.player_damage > 0) ? game::flash_strength(since_hit) : 0.0f;
    draw_sprite(sheet, game::Sprite::Player, player_pos, hit_tint(player_flash));

    draw_hud(s, seed);
    if (!view.active) draw_end_screen(s, sim::outcome_of(s));
}

}  // namespace

int main(int argc, char** argv) {
    std::uint64_t seed = argc > 1 ? std::strtoull(argv[1], nullptr, 10) : game::clock_seed();

    auto state = sim::parse_map(game::kLevel, seed);
    if (!state) {
        std::cerr << "invalid level\n";
        return 1;
    }

    const game::Size size = game::window_size(state->width, state->height, kTilePx, kHudPx);
    InitWindow(size.w, size.h, "Delve");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);  // we handle quitting ourselves (q), not raylib's default Escape

    // The assets are copied next to the executable at build time, so this works from any cwd.
    const std::string sheet_path = std::string(GetApplicationDirectory()) + "assets/tilemap_packed.png";
    const Texture2D sheet = LoadTexture(sheet_path.c_str());
    if (sheet.id == 0) {
        std::cerr << "cannot load " << sheet_path << "\n";
        CloseWindow();
        return 1;
    }
    SetTextureFilter(sheet, TEXTURE_FILTER_POINT);

    TurnView view;
    std::optional<sim::Action> pending;  // one buffered key press; a newer one replaces it

    bool quit = false;
    while (!quit && !WindowShouldClose()) {
        const game::Key key = poll_key();
        if (key == game::Key::Quit) {
            quit = true;
        } else if (key == game::Key::Restart) {
            seed = sim::splitmix64(seed);
            state = sim::parse_map(game::kLevel, seed);
            view = TurnView{};
            pending.reset();
        } else {
            sim::Action action;
            if (game::to_action(key, action)) pending = action;
        }

        if (view.active) {
            view.elapsed += GetFrameTime();
            if (view.elapsed >= game::kTurnSeconds) view.active = false;
        }
        if (!view.active && pending) {
            // A finished game ignores further moves, so there is nothing to animate.
            if (sim::outcome_of(*state) == sim::Outcome::Ongoing) {
                const sim::State before = *state;
                sim::step(*state, *pending);
                view.delta = game::make_delta(before, *state);
                view.elapsed = 0.0f;
                view.active = true;
            }
            pending.reset();
        }

        BeginDrawing();
        draw(*state, seed, sheet, view);
        EndDrawing();
    }
    UnloadTexture(sheet);
    CloseWindow();
    return 0;
}
