#include "sim/rng.hpp"

namespace sim {

namespace {
std::uint64_t rotl(std::uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }
}  // namespace

std::uint64_t splitmix64(std::uint64_t& x) {
    x += 0x9e3779b97f4a7c15ULL;
    std::uint64_t z = x;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

RngState make_rng(std::uint64_t seed) {
    RngState s;
    for (auto& word : s) word = splitmix64(seed);
    return s;
}

std::uint64_t next_u64(RngState& s) {
    const std::uint64_t result = rotl(s[1] * 5, 7) * 9;
    const std::uint64_t t = s[1] << 17;
    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= t;
    s[3] = rotl(s[3], 45);
    return result;
}

std::uint32_t range(RngState& s, std::uint32_t n) {
    // Reject the first (2^64 mod n) values so every residue is equally likely.
    const std::uint64_t threshold = (0 - static_cast<std::uint64_t>(n)) % n;
    std::uint64_t r = next_u64(s);
    while (r < threshold) r = next_u64(s);
    return static_cast<std::uint32_t>(r % n);
}

}  // namespace sim
