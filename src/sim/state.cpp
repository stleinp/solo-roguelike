#include "sim/state.hpp"

#include <cstddef>

namespace sim {

std::optional<State> parse_map(std::string_view text) {
    State s;
    int players = 0;
    int enemies = 0;
    int goals = 0;

    std::size_t pos = 0;
    while (pos < text.size()) {
        std::size_t end = text.find('\n', pos);
        if (end == std::string_view::npos) end = text.size();
        std::string_view row = text.substr(pos, end - pos);
        pos = end + 1;

        if (s.height == 0) s.width = static_cast<int>(row.size());
        if (row.empty() || static_cast<int>(row.size()) != s.width) return std::nullopt;

        for (int x = 0; x < s.width; ++x) {
            const Pos p{x, s.height};
            Tile t = Tile::Floor;
            switch (row[static_cast<std::size_t>(x)]) {
                case '#':
                    t = Tile::Wall;
                    break;
                case '.':
                    break;
                case '>':
                    t = Tile::Goal;
                    ++goals;
                    break;
                case '@':
                    s.player = p;
                    ++players;
                    break;
                case 'E':
                    s.enemy = p;
                    ++enemies;
                    break;
                default:
                    return std::nullopt;
            }
            s.tiles.push_back(t);
        }
        ++s.height;
    }

    if (players != 1 || enemies != 1 || goals < 1) return std::nullopt;
    return s;
}

}  // namespace sim
