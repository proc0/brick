cmake_minimum_required(VERSION 3.21)
project(brick
    VERSION 0.1.0
    DESCRIPTION "A UI library extension for Clay."
    LANGUAGES C CXX          # CXX is needed only for the C++ compat test
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
#  Fetch Clay
# -------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    clay
    GIT_REPOSITORY  https://github.com/nicbarker/clay.git
    GIT_TAG         v0.14         # pin to a tag, not a branch
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(clay)
# Clay's CMake exposes an INTERFACE target called `clay` (verify name
# in Clay's CMakeLists.txt; it may be `Clay::Clay`).

# -------------------------------------------------------------------
#  Brick – INTERFACE (header-only) library
# -------------------------------------------------------------------
add_library(brick INTERFACE)
add_library(Brick::Brick ALIAS brick)

target_include_directories(brick INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Propagate Clay so consumers get its include path for free
target_link_libraries(brick INTERFACE clay)

target_compile_features(brick INTERFACE c_std_99)

# -------------------------------------------------------------------
#  Options
# -------------------------------------------------------------------
option(BRICK_BUILD_TESTS    "Build unit tests"        ON)
option(BRICK_BUILD_EXAMPLES "Build example programs"   ON)
option(BRICK_RENDERER_OPENGL "Enable OpenGL renderer"  OFF)
option(BRICK_RENDERER_SDL2   "Enable SDL2 renderer"    OFF)

# -------------------------------------------------------------------
#  Tests
# -------------------------------------------------------------------
if(BRICK_BUILD_TESTS)
    enable_testing()

    # ---- C99 test binary ----
    add_executable(brick_test_c99
        tests/brick_test_impl.c
        tests/test_state.c
        tests/test_buttons.c
        tests/test_tabs.c
        tests/test_scroll.c
        tests/test_runner.c
    )
    target_link_libraries(brick_test_c99 PRIVATE brick)
    target_include_directories(brick_test_c99 PRIVATE tests/)
    target_compile_options(brick_test_c99 PRIVATE -Wall -Wextra -Werror -pedantic)
    add_test(NAME brick_c99 COMMAND brick_test_c99)

    # ---- C++20 compatibility test (same sources, C++ compiler) ----
    set_source_files_properties(
        tests/brick_test_impl.c
        tests/test_state.c
        tests/test_buttons.c
        tests/test_tabs.c
        tests/test_scroll.c
        tests/test_runner.c
        PROPERTIES LANGUAGE CXX
    )
    add_executable(brick_test_cpp
        tests/brick_test_impl.c
        tests/test_state.c
        tests/test_buttons.c
        tests/test_tabs.c
        tests/test_scroll.c
        tests/test_runner.c
    )
    target_link_libraries(brick_test_cpp PRIVATE brick)
    target_include_directories(brick_test_cpp PRIVATE tests/)
    target_compile_options(brick_test_cpp PRIVATE -Wall -Wextra -Werror)
    add_test(NAME brick_cpp20 COMMAND brick_test_cpp)

    # ---- ASan / UBSan build ----
    add_executable(brick_test_asan
        tests/brick_test_impl.c
        tests/test_state.c
        tests/test_buttons.c
        tests/test_tabs.c
        tests/test_scroll.c
        tests/test_runner.c
    )
    target_link_libraries(brick_test_asan PRIVATE brick)
    target_include_directories(brick_test_asan PRIVATE tests/)
    target_compile_options(brick_test_asan PRIVATE
        -Wall -Wextra -Werror -fsanitize=address,undefined
    )
    target_link_options(brick_test_asan PRIVATE -fsanitize=address,undefined)
    add_test(NAME brick_asan COMMAND brick_test_asan)
endif()

# -------------------------------------------------------------------
#  Examples
# -------------------------------------------------------------------
if(BRICK_BUILD_EXAMPLES)
    add_subdirectory(examples/button_demo)
    add_subdirectory(examples/tab_demo)
    add_subdirectory(examples/scroll_demo)
endif()

# -------------------------------------------------------------------
#  Renderers (optional, require external deps)
# -------------------------------------------------------------------
if(BRICK_RENDERER_OPENGL)
    add_subdirectory(renderers/opengl)
endif()
if(BRICK_RENDERER_SDL2)
    add_subdirectory(renderers/sdl2)
endif()

# -------------------------------------------------------------------
#  Install (so downstream CMake projects can find_package(brick))
# -------------------------------------------------------------------
include(GNUInstallDirs)
install(FILES brick.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# (Clay's install is handled by Clay's own CMake via FetchContent;
#  if you install Brick, you should also `install` Clay's header or
#  document that the user must provide it.)
