#This file was made by gemini bc I hate cmake

cmake_minimum_required(VERSION 3.15)
project(tank-game CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES main.cpp)

# Output executable target
add_executable(tank-game ${SOURCES})

# Target file extension must be .html for Emscripten to generate the runner
set_target_properties(tank-game PROPERTIES SUFFIX ".html")

# Emscripten-specific build flags
if(EMSCRIPTEN)
    target_compile_options(tank-game PRIVATE
        -O2
        -Wall
    )

    target_link_options(tank-game PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_SDL=2"
        "-sFULL_ES3=1"
        "-sMIN_WEBGL_VERSION=2"
        "-sMAX_WEBGL_VERSION=2"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/template.html"
    )

endif()
