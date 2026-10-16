cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME brick_examples_raylib_website)
project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 99)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(BUILD_WEB ON)

# LOCAL_EM_INCLUDE - Emscripten Include path REQUIRED for WEB
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(LOCAL_EM_INCLUDE "C:/emsdk/upstream/emscripten/cache/sysroot/include")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(LOCAL_EM_INCLUDE "/usr/local/lib/emsdk/upstream/emscripten/cache/sysroot/include")
else()
    set(LOCAL_EM_INCLUDE "/usr/local/lib/emsdk/upstream/emscripten/cache/sysroot/include")
endif()

if(BUILD_WEB AND NOT EXISTS "${LOCAL_EM_INCLUDE}")
    message(FATAL_ERROR "SETTINGS ERROR: Emscripten include path not found!: ${LOCAL_EM_INCLUDE}")
endif()

# Adding Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "6.0"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

# don't build the supplied examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CUSTOMIZE_BUILD OFF CACHE BOOL "" FORCE) 

FetchContent_MakeAvailable(raylib)

if(BUILD_WEB)
  add_executable(${PROJECT_NAME} main.c)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  target_include_directories(${PROJECT_NAME} PRIVATE "${LOCAL_EM_INCLUDE}")

  if(BUILD_DEBUG)
      target_compile_options(raylib PRIVATE 
          -O0
      )

      target_compile_options(${PROJECT_NAME} PRIVATE 
          -fwasm-exceptions
      )
  else()
    target_compile_options(raylib PRIVATE 
        -O2
    )
  endif()

  target_compile_options(raylib PRIVATE 
      -Wall 
      -DPLATFORM_WEB 
      -DGRAPHICS_API_OPENGL_ES3
  )

  if(BUILD_DEBUG)
      set(FILE_WEB_SHELL "shell.html")
  else()
      set(FILE_WEB_SHELL "minshell.html")
  endif()

  set(PATH_WEB_SHELL "${CMAKE_SOURCE_DIR}/build-web/_deps/raylib-src/src/${FILE_WEB_SHELL}")

  # FULL_ES3 requires Raylib to be compiled for Web with OpenGL ES3 (see README requirements)
  target_link_options(${PROJECT_NAME} PRIVATE 
      -sFULL_ES3 
      -sUSE_GLFW=3 
      -sWASM=1
      # asyncify is needed to use emscripten_sleep and async functions to automatically adapt raylib infinite loop to web
      # this can be removed if you handle the infinite loop with emscripten_set_main_loop_arg
      -sASYNCIFY=1
      # 12MB
      # -sINITIAL_HEAP=100663296
      # ~3MB
      # -sINITIAL_HEAP=25165824
      # ~8MB
      # -sINITIAL_HEAP=67108864
      -sEXPORT_ES6=1
      -sEXPORTED_RUNTIME_METHODS=["ccall","cwrap","HEAPF32","requestFullscreen"]
      -sEXPORTED_FUNCTIONS=_main
      --preload-file "${CMAKE_CURRENT_SOURCE_DIR}/resources@resources"
      --shell-file "${PATH_WEB_SHELL}"
  )

  if(BUILD_DEBUG)
      target_link_options(${PROJECT_NAME} PRIVATE 
          # wasm-exceptinos not cmpatible with ASYNCIFY
          -fwasm-exceptions
          -sABORT_ON_WASM_EXCEPTIONS
          --emrun
      )
  else()
      target_link_options(${PROJECT_NAME} PRIVATE 
          -sMINIFY_HTML
      )
  endif()

  target_link_libraries(${PROJECT_NAME} PRIVATE raylib Brick::Brick)

  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/dist-web" COMPONENT Runtime)
  install(FILES 
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.js"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.wasm"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.data"
      DESTINATION "${PROJECT_SOURCE_DIR}/dist-web" COMPONENT Runtime)

else()

  add_executable(${PROJECT_NAME} main.c)
  target_include_directories(${PROJECT_NAME} PRIVATE .)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib Brick::Brick)

  if(MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
  else()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
  endif()

  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources")
  message(STATUS "\nCopying ${CMAKE_CURRENT_SOURCE_DIR}/resources")
  add_custom_command(
          TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/resources
          ${CMAKE_CURRENT_BINARY_DIR}/resources)
  endif()

endif()


