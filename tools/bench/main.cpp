// Throughput benchmark: plays random episodes and prints steps per second.
// Always measure in a Release build (see docs/omgeving.md).
//
//   bench [episodes]        (default 20000)

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "sim/hash.hpp"
#include "sim/step.hpp"

namespace {

constexpr const char* kMap =
    "###########\n"
    "#@....#...#\n"
    "#.###.#.#.#\n"
    "#.#...#.#.#\n"
    "#.#.###.#.#\n"
    "#...E...#>#\n"
    "###########\n";

}  // namespace

int main(int argc, char** argv) {
#ifndef NDEBUG
    std::cerr << "WARNING: not a Release build; the number below is meaningless.\n";
#endif

    const std::uint64_t episodes = argc > 1 ? std::strtoull(argv[1], nullptr, 10) : 20000;

    const auto initial = sim::parse_map(kMap);
    if (!initial) {
        std::cerr << "invalid map\n";
        return 1;
    }

    std::uint64_t steps = 0;
    std::uint64_t won = 0, lost = 0, timeout = 0;
    std::uint64_t checksum = 0;  // depends on every episode, so nothing can be optimised away

    const auto start = std::chrono::steady_clock::now();
    for (std::uint64_t e = 0; e < episodes; ++e) {
        sim::State s = *initial;  // reset = copy of the initial state
        s.rng_state = sim::make_rng(e);
        sim::RngState actions = sim::make_rng(e + 0x1234567);

        sim::StepResult r;
        while (!r.done()) {
            r = sim::step(s, static_cast<sim::Action>(sim::range(actions, sim::kActionCount)));
            ++steps;
        }
        switch (r.outcome) {
            case sim::Outcome::Won:
                ++won;
                break;
            case sim::Outcome::Lost:
                ++lost;
                break;
            case sim::Outcome::Timeout:
                ++timeout;
                break;
            case sim::Outcome::Ongoing:
                break;
        }
        checksum ^= sim::hash(s);
    }
    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;

    std::cout << "episodes:  " << episodes << "  (won " << won << ", lost " << lost << ", timeout "
              << timeout << ")\n"
              << "steps:     " << steps << "\n"
              << "time:      " << elapsed.count() << " s\n"
              << "steps/s:   "
              << static_cast<std::uint64_t>(static_cast<double>(steps) / elapsed.count()) << "\n"
              << "checksum:  0x" << std::hex << checksum << "\n";
    return 0;
}
