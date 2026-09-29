#include "game/terminal.hpp"

#include <termios.h>
#include <unistd.h>

#include <cstdio>

namespace game {

struct Terminal::Saved {
    termios original;
};

namespace {

void write_all(const std::string& s) {
    std::size_t done = 0;
    while (done < s.size()) {
        const ssize_t n = ::write(STDOUT_FILENO, s.data() + done, s.size() - done);
        if (n <= 0) return;
        done += static_cast<std::size_t>(n);
    }
}

bool read_byte(char& c) { return ::read(STDIN_FILENO, &c, 1) == 1; }

}  // namespace

bool Terminal::is_interactive() { return ::isatty(STDIN_FILENO) && ::isatty(STDOUT_FILENO); }

Terminal::Terminal() : saved_(new Saved) {
    ::tcgetattr(STDIN_FILENO, &saved_->original);
    termios raw = saved_->original;
    raw.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO | ISIG));
    raw.c_iflag &= static_cast<tcflag_t>(~(IXON | ICRNL));
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    ::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    write_all("\x1b[?1049h\x1b[?25l");  // alternate screen, hide cursor
}

Terminal::~Terminal() {
    write_all("\x1b[?25h\x1b[?1049l");  // show cursor, leave alternate screen
    ::tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_->original);
    delete saved_;
}

void Terminal::draw(const std::string& text) {
    std::string out = "\x1b[H\x1b[2J";  // cursor home, clear screen
    for (char c : text) {
        if (c == '\n') out += '\r';
        out += c;
    }
    write_all(out);
}

Key Terminal::read_key() {
    char c;
    if (!read_byte(c)) return Key::Quit;
    switch (c) {
        case 'w':
        case 'k':
            return Key::Up;
        case 's':
        case 'j':
            return Key::Down;
        case 'a':
        case 'h':
            return Key::Left;
        case 'd':
        case 'l':
            return Key::Right;
        case ' ':
        case '.':
            return Key::Wait;
        case 'r':
            return Key::Restart;
        case 'q':
        case 3:
            return Key::Quit;  // 3 = Ctrl-C
        case 27: {             // escape sequence: arrow keys are ESC [ A..D
            char a, b;
            if (!read_byte(a) || a != '[' || !read_byte(b)) return Key::Other;
            switch (b) {
                case 'A':
                    return Key::Up;
                case 'B':
                    return Key::Down;
                case 'C':
                    return Key::Right;
                case 'D':
                    return Key::Left;
                default:
                    return Key::Other;
            }
        }
        default:
            return Key::Other;
    }
}

}  // namespace game
