#pragma once

#include <string>

#include "game/input.hpp"

namespace game {

// Raw-mode terminal on the alternate screen. The constructor switches it on and the
// destructor restores the original state, so the terminal is left intact on any normal exit.
// Ctrl-C is read as a key (Quit) instead of a signal, so the destructor always runs.
class Terminal {
   public:
    Terminal();
    ~Terminal();
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    static bool is_interactive();  // stdin and stdout are both terminals

    void draw(const std::string& text);
    Key read_key();  // blocks; end of input counts as Quit

   private:
    struct Saved;
    Saved* saved_;
};

}  // namespace game
