#include <doctest/doctest.h>

#include "sim/sim.hpp"

TEST_CASE("sim library links") { CHECK(sim::version() == 0); }
