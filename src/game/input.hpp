#pragma once

#include "sim/types.hpp"

namespace game {

// Frontend-independent input: each frontend (terminal, window) translates its own key
// codes into this and the rest of the game only sees Key.
enum class Key { Up, Down, Left, Right, Wait, Restart, Quit, Other };

// Maps a movement or wait key to a simulation action. Returns false for the other keys.
bool to_action(Key k, sim::Action& a);

}  // namespace game
