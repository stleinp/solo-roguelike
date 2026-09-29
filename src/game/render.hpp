#pragma once

#include <cstdint>
#include <string>

#include "sim/state.hpp"

namespace game {

// Draws the state as text: map, status line and a message or the controls. Pure function of
// its inputs; the terminal layer decides where the text ends up. With color = true the
// output contains ANSI colour codes.
std::string render(const sim::State& s, std::uint64_t seed, bool color);

}  // namespace game
