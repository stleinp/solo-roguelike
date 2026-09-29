#pragma once

#include <array>
#include <cstdint>

namespace sim {

// xoshiro256** with splitmix64 seeding (B14). Own implementation, no <random>: the standard
// distributions are not portable across standard libraries. The state lives in State.
using RngState = std::array<std::uint64_t, 4>;

// One splitmix64 step; advances x and returns the output.
std::uint64_t splitmix64(std::uint64_t& x);

// Expands a 64-bit seed into a full state (never all zeros).
RngState make_rng(std::uint64_t seed);

std::uint64_t next_u64(RngState& s);

// Uniform integer in [0, n), without modulo bias. n must be > 0.
std::uint32_t range(RngState& s, std::uint32_t n);

}  // namespace sim
