#pragma once

#include <cstdint>

#include "sim/state.hpp"

namespace sim {

// FNV-1a (64 bit) over every field of the State, each integer fed as fixed-width
// little-endian bytes, so the value does not depend on endianness or struct padding.
// Two states hash equal only if they are identical (up to hash collisions). Used by the
// determinism test (B6, B14) and later to detect diverging replays.
std::uint64_t hash(const State& s);

}  // namespace sim
