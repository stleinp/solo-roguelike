#include "game/input.hpp"

namespace game {

bool to_action(Key k, sim::Action& a) {
    switch (k) {
        case Key::Up:
            a = sim::Action::Up;
            return true;
        case Key::Down:
            a = sim::Action::Down;
            return true;
        case Key::Left:
            a = sim::Action::Left;
            return true;
        case Key::Right:
            a = sim::Action::Right;
            return true;
        case Key::Wait:
            a = sim::Action::Wait;
            return true;
        default:
            return false;
    }
}

}  // namespace game
