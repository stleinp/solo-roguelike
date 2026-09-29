# Presentation layer (B5): reads the simulation, sends actions back. sim never links to this.
add_library(render STATIC render.cpp input.cpp layout.cpp anim.cpp)
target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(render PUBLIC sim PRIVATE delve_options)

add_executable(delve main.cpp terminal.cpp)
target_link_libraries(delve PRIVATE render sim delve_options)

# Windowed frontend (B23). raylib is only linked here: never by sim, bench, tests or render.
# Switch off with -DDELVE_BUILD_GFX=OFF where the X11/OpenGL dev packages are missing.
option(DELVE_BUILD_GFX "Build the windowed frontend delve_gfx (fetches raylib)" ON)
if(DELVE_BUILD_GFX)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5)
    FetchContent_MakeAvailable(raylib)

    add_executable(delve_gfx gfx_main.cpp)
    target_link_libraries(delve_gfx PRIVATE render sim raylib delve_options)

    # The executable looks for assets/ next to itself (any working directory).
    add_custom_command(TARGET delve_gfx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:delve_gfx>/assets)
endif()
