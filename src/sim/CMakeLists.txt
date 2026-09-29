# The simulation: game rules, state, RNG. No I/O, links against nothing (B5).
add_library(sim STATIC state.cpp step.cpp rng.cpp)
target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(sim PRIVATE delve_options)
