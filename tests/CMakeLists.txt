include(FetchContent)
FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11)
FetchContent_MakeAvailable(doctest)

add_executable(tests main.cpp test_state.cpp test_step.cpp test_rng.cpp test_enemy.cpp test_determinism.cpp test_render.cpp test_layout.cpp test_anim.cpp)
target_link_libraries(tests PRIVATE render sim delve_options doctest::doctest)

list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)
include(doctest)
doctest_discover_tests(tests)
