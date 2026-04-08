cmake_minimum_required(VERSION 3.14)
project(chess_game C)

# --- Raylib Configuration ---
include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PLATFORM "PLATFORM_WEB" CACHE STRING "" FORCE)

# Use raylib 5.5
FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
)
FetchContent_MakeAvailable(raylib)

# --- Game Executable ---
set(SOURCES
    src/main.c
    src/board.c
    src/render.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Include local src headers
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# --- Web/Emscripten Specific Settings ---
if(EMSCRIPTEN)
    # Generate .html output
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    # Link options for Emscripten
    target_link_options(${PROJECT_NAME} PRIVATE
        "-sUSE_GLFW=3"
        "-sASYNCIFY"
        "-sALLOW_MEMORY_GROWTH=1"
        "--shell-file" "${CMAKE_SOURCE_DIR}/src/minshell.html"
        "--preload-file" "${CMAKE_SOURCE_DIR}/src/@src"
        "-O3"
    )
endif()
