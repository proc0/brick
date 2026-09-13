cmake_minimum_required(VERSION 3.27)
set(PROJECT_VERSION "0.1.0")

# ============================================================================
#  Brick — Render-agnostic UI components for Clay
#  Header-only, STB-style. C99 + C++20.
#
#  Build (developer):
#    cmake -B build (default) 
#    --OR--
#    cmake -B build -S . -G "MinGW Makefiles" (MSYS2 aka MinGW aka GCC on Windows)
#    
#    cmake --build build
#    ctest --test-dir build --output-on-failure
#
#  Install:
#    cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local
#    cmake --build build
#    cmake --install build
#
#  Use (downstream, CMake):
#    FetchContent_Declare(brick GIT_REPOSITORY ... GIT_TAG ...)
#    FetchContent_MakeAvailable(brick)
#    target_link_libraries(my_app PRIVATE Brick::Brick)
#
#  Use (downstream, no CMake):
#    #include "brick.h"          (with BRICK_IMPLEMENTATION in one TU)
#    #include "renderers/..."   (optional, from Clay)
# ============================================================================

project(brick
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "A UI library extension for Clay."
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD          99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD        20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release for examples
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ============================================================================
#                                 DEPENDENCY
# ============================================================================
# Clay's CMakeLists does not define a library target, it only builds
# examples. This only pulls Clay source by defining an empty subdir.
# NOTE: Brick includes Clay: brick.h defines CLAY_IMPLEMENTATION internally
# before #include "clay.h", so the user only manages Brick.

include(FetchContent)
FetchContent_Declare(
    clay
    GIT_REPOSITORY https://github.com/nicbarker/clay.git
    GIT_TAG        head # TODO: update to a tag once Clay fixes incosistencies. Ticket: https://github.com/nicbarker/clay/issues/655
    GIT_PROGRESS   TRUE 
    GIT_SHALLOW    TRUE
    # Point to a directory that does not exist, preventing add_subdirectory()
    SOURCE_SUBDIR nonexistent_dir
)
FetchContent_MakeAvailable(clay)

# ============================================================================
#                                 LIBRARY
# ============================================================================
# Wrap Clay as a target because Clay doesn't create a CMake target.
# This target is only to propagate the include directory so that
# `target_link_libraries(x PRIVATE Clay::Clay)` makes clay.h and
# renderers/ visible to the consumer.

add_library(clay INTERFACE)
add_library(Clay::Clay ALIAS clay)
# clay_SOURCE_DIR → clay.h, renderers/, etc.
target_include_directories(clay INTERFACE ${clay_SOURCE_DIR})
target_compile_features(clay INTERFACE c_std_99)

# BRICK INTERFACE
add_library(brick INTERFACE)
add_library(Brick::Brick ALIAS brick)

# Header-only. No compiled artifact.
#  - the include directory (root)
#  - a dependency on Clay (clay.h + renderers/)
# Downstream users link Brick::Brick and get both include paths.
target_include_directories(brick INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   # dev: repo root
    $<INSTALL_INTERFACE:include>                     # installed: <prefix>/include
)

# propagates clay include dir
target_link_libraries(brick INTERFACE Clay::Clay)
target_compile_features(brick INTERFACE c_std_99)


# ============================================================================
#                                 OPTIONS
# ============================================================================

option(BRICK_BUILD_TESTS "Build unit tests" OFF)
option(BRICK_BUILD_EXAMPLES "Build examples" ON)
# Generate compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# 4. TESTS
#
#    Three binaries from the same sources:
#      - C99       : language conformance
#      - C++20     : extern "C" / compilation in C++ mode
#      - ASan+UBSan: memory safety (global state, init/shutdown cycles)
#
#    Structure:
#      tests/brick_test_impl.c  →  #define BRICK_IMPLEMENTATION / #include "brick.h"
#      tests/test_*.c           →  minunit test cases (API only, no impl)
#      tests/test_runner.c      →  main() + MU_TEST_SUITE wiring
#      tests/minunit.h          →  vendored, single file
# ============================================================================

if(BRICK_BUILD_TESTS)
    enable_testing()

    set(BRICK_TEST_SOURCES
        tests/brick_test_impl.c
        tests/test_state.c
        tests/test_buttons.c
        tests/test_tabs.c
        tests/test_scroll.c
        tests/test_runner.c
    )

    # ---- C99 ---------------------------------------------------------------
    add_executable(brick_test_c99 ${BRICK_TEST_SOURCES})
    target_link_libraries(brick_test_c99 PRIVATE brick)
    target_include_directories(brick_test_c99 PRIVATE tests/)
    target_compile_options(brick_test_c99 PRIVATE -Wall -Wextra -Werror -pedantic)
    add_test(NAME c99 COMMAND brick_test_c99)

    # ---- C++20 (same .c files, compiled as C++) ----------------------------
    # set_source_files_properties(${BRICK_TEST_SOURCES} PROPERTIES LANGUAGE CXX)
    # add_executable(brick_test_cpp20 ${BRICK_TEST_SOURCES})
    # target_link_libraries(brick_test_cpp20 PRIVATE brick)
    # target_include_directories(brick_test_cpp20 PRIVATE tests/)
    # target_compile_options(brick_test_cpp20 PRIVATE -Wall -Wextra -Werror)
    # add_test(NAME cpp20 COMMAND brick_test_cpp20)

    # TODO:
    # ---- ASan + UBSan ------------------------------------------------------
    # add_executable(brick_test_asan ${BRICK_TEST_SOURCES})
    # target_link_libraries(brick_test_asan PRIVATE brick)
    # target_include_directories(brick_test_asan PRIVATE tests/)
    # target_compile_options(brick_test_asan PRIVATE
    #     -Wall -Wextra -Werror
    #     -fsanitize=address,undefined
    #     -fno-sanitize-recover=all
    # )
    # target_link_options(brick_test_asan PRIVATE
    #     -fsanitize=address,undefined
    # )
    # add_test(NAME asan COMMAND brick_test_asan)
endif()


# ============================================================================
# 5. EXAMPLES
#
#    Each example is a self-contained executable. The pattern in every
#    main.c is:
#
#        #define BRICK_IMPLEMENTATION
#        #include "brick.h"                          ← pulls in Clay (impl)
#
#        #include "renderers/<name>/<name>.h"        ← include Clay renderer
#
#    Since Clay renderers do NOT #include "clay.h" themselves, they rely
#    on it being already visible — which it is, because brick.h includes
#    clay.h (with implementation) before the renderer is included.
#
#    Render-agnostic examples (no external deps) always build.
#    Renderer-backed examples build only if the external library is found.
# ============================================================================

if(BRICK_BUILD_EXAMPLES)

    # --- Always builds: no renderer, just exercises Brick's API headlessly --
    add_subdirectory(examples/raylib-simple)

    # --- OpenGL renderer (Clay's) -------------------------------------------
    # find_package(OpenGL QUIET)
    # if(OpenGL_FOUND)
    #     add_subdirectory(examples/button_demo)
    #     add_subdirectory(examples/tab_demo)
    # endif()

    # # --- SDL2 renderer (Clay's) ---------------------------------------------
    # find_package(SDL2 QUIET)
    # if(SDL2_FOUND)
    #     add_subdirectory(examples/scroll_demo)
    # endif()

endif()


# ============================================================================
# 6. INSTALL
#
#    Produces a flat include tree:
#
#      <prefix>/include/
#      ├── brick.h
#      ├── clay.h
#      └── renderers/
#          ├── opengl_renderer/
#          │   └── opengl_renderer.h
#          ├── sdl2_renderer/
#          │   └── sdl2_renderer.h
#          └── ...
#
#    This matches the FetchContent layout and the copy/paste layout,
#    so user source code is identical across all three usage scenarios.
# ============================================================================

# include(GNUInstallDirs)

# install(FILES
#     brick.h
#     ${clay_SOURCE_DIR}/clay.h
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# # Clay renderers: install all .h files, preserve subdirectory structure
# install(DIRECTORY
#     ${clay_SOURCE_DIR}/renderers/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/renderers
#     FILES_MATCHING PATTERN "*.h"
# )

# (No .a / .so to install. No CMake config files needed for the
#  FetchContent path. If you later want find_package(Brick) to work
#  for installed copies, add a BrickConfig.cmake.in + configure_package_config_file.)


# ============================================================================
# 7. PACKAGE CONFIG (optional, for find_package(Brick) on installed builds)
#
#    Only needed if users install Brick and then use find_package in a
#    downstream CMake project instead of FetchContent. Harmless to include.
# ============================================================================

# include(CMakePackageConfigHelpers)

# Generate BrickTargets.cmake (exports Brick::Brick + Clay::Clay)
# install(TARGETS brick clay
#     EXPORT BrickTargets
#     NAMESPACE Brick::
# )

# Hmm — the targets have different namespaces (Brick::Brick vs Clay::Clay).
# Simpler: just export `brick` and let the alias resolve at use-time.
# Actually, for INTERFACE libs the export is trivial. Let's keep it minimal:

# install(EXPORT BrickTargets
#     FILE        BrickTargets.cmake
#     NAMESPACE   Brick::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Brick
# )

# write_basic_package_config_file(
#     ${CMAKE_CURRENT_SOURCE_DIR}/cmake/BrickConfig.cmake.in
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Brick
#     PACKAGE_NAME Brick
# )

# (Create cmake/BrickConfig.cmake.in — see note below this block.)
