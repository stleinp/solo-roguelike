#include "game/render.hpp"

#include <string_view>

#include "sim/step.hpp"

namespace game {

namespace {

constexpr std::string_view kReset = "\x1b[0m";
constexpr std::string_view kGray = "\x1b[90m";
constexpr std::string_view kGreen = "\x1b[1;32m";
constexpr std::string_view kYellow = "\x1b[1;33m";
constexpr std::string_view kRed = "\x1b[1;31m";

void put(std::string& out, char c, std::string_view color, bool use_color) {
    if (use_color && !color.empty()) out += color;
    out += c;
    if (use_color && !color.empty()) out += kReset;
}

}  // namespace

std::string render(const sim::State& s, std::uint64_t seed, bool color) {
    std::string out = "DELVE\n\n";

    for (int y = 0; y < s.height; ++y) {
        for (int x = 0; x < s.width; ++x) {
            const sim::Pos p{x, y};
            if (p == s.player) {
                put(out, '@', kYellow, color);
            } else if (s.enemy_alive() && p == s.enemy) {
                put(out, 'E', kRed, color);
            } else {
                switch (s.tile_at(p)) {
                    case sim::Tile::Wall:
                        put(out, '#', kGray, color);
                        break;
                    case sim::Tile::Goal:
                        put(out, '>', kGreen, color);
                        break;
                    case sim::Tile::Floor:
                        put(out, '.', "", color);
                        break;
                }
            }
        }
        out += '\n';
    }

    out += "\nHP " + std::to_string(s.player_hp) + "/" + std::to_string(sim::kPlayerStartHp);
    out += "   Enemy ";
    out += s.enemy_alive() ? std::to_string(s.enemy_hp) + "/" + std::to_string(sim::kEnemyStartHp)
                           : "dead";
    out += "   Turn " + std::to_string(s.turn) + "/" + std::to_string(sim::kMaxTurns);
    out += "   Seed " + std::to_string(seed) + "\n\n";

    switch (sim::outcome_of(s)) {
        case sim::Outcome::Won:
            out += "You reached the goal!  [r] new game  [q] quit\n";
            break;
        case sim::Outcome::Lost:
            out += "You died.  [r] new game  [q] quit\n";
            break;
        case sim::Outcome::Timeout:
            out += "Out of time.  [r] new game  [q] quit\n";
            break;
        case sim::Outcome::Ongoing:
            out += "Move: arrows / wasd / hjkl   Wait: space   Restart: r   Quit: q\n";
            out += "Walk into the enemy to attack it. Reach > to win.\n";
            break;
    }
    return out;
}

}  // namespace game
