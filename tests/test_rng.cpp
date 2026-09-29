#include <doctest/doctest.h>

#include "sim/rng.hpp"

using namespace sim;

TEST_CASE("splitmix64 matches the reference output for seed 0") {
    std::uint64_t x = 0;
    CHECK(splitmix64(x) == 0xe220a8397b1dcdafULL);
}

TEST_CASE("make_rng is deterministic and seed-dependent") {
    CHECK(make_rng(42) == make_rng(42));
    CHECK_FALSE(make_rng(42) == make_rng(43));
    CHECK_FALSE(make_rng(0) == RngState{});
}

TEST_CASE("same seed gives the same stream") {
    RngState a = make_rng(7);
    RngState b = make_rng(7);
    for (int i = 0; i < 100; ++i) CHECK(next_u64(a) == next_u64(b));
}

TEST_CASE("a copied state replays identically") {
    RngState a = make_rng(7);
    next_u64(a);
    RngState copy = a;
    CHECK(next_u64(a) == next_u64(copy));
}

TEST_CASE("range stays in bounds and n == 1 gives 0") {
    RngState s = make_rng(1);
    for (int i = 0; i < 1000; ++i) {
        CHECK(range(s, 1) == 0);
        CHECK(range(s, 6) < 6);
    }
}

TEST_CASE("range is roughly uniform") {
    RngState s = make_rng(123);
    int counts[4] = {};
    for (int i = 0; i < 10000; ++i) ++counts[range(s, 4)];
    for (int c : counts) {
        CHECK(c > 2200);
        CHECK(c < 2800);
    }
}
