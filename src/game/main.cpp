#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "game/input.hpp"
#include "game/level.hpp"
#include "game/render.hpp"
#include "game/terminal.hpp"
#include "sim/step.hpp"

int main(int argc, char** argv) {
    if (!game::Terminal::is_interactive()) {
        std::cerr << "delve needs an interactive terminal.\n";
        return 1;
    }

    std::uint64_t seed = argc > 1 ? std::strtoull(argv[1], nullptr, 10) : game::clock_seed();

    auto state = sim::parse_map(game::kLevel, seed);
    if (!state) {
        std::cerr << "invalid level\n";
        return 1;
    }

    game::Terminal term;
    for (;;) {
        term.draw(game::render(*state, seed, true));

        const game::Key key = term.read_key();
        if (key == game::Key::Quit) break;
        if (key == game::Key::Restart) {
            seed = sim::splitmix64(seed);
            state = sim::parse_map(game::kLevel, seed);
            continue;
        }
        sim::Action action;
        if (game::to_action(key, action)) sim::step(*state, action);
    }
    return 0;
}
