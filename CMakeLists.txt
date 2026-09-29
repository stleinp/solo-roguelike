cmake_minimum_required(VERSION 3.25)
project(delve LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Warnings and sanitizers are applied per target through this interface library,
# so that third-party code (doctest) is not held to our warning level.
add_library(delve_options INTERFACE)
target_compile_options(delve_options INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
target_compile_options(delve_options INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer>)
target_link_options(delve_options INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>)

add_subdirectory(src/sim)
add_subdirectory(src/game)
add_subdirectory(tools/bench)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
