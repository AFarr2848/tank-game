#This file was made by clade bc I hate cmake

cmake_minimum_required(VERSION 3.15)
project(tank-game CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Maze.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/ShaderManager.cpp

)

# Output executable target
add_executable(tank-game ${SOURCES})

# Target file extension must be .html for Emscripten to generate the runner
set_target_properties(tank-game PROPERTIES SUFFIX ".html")

file(GLOB_RECURSE SHADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*"
)

set_target_properties(tank-game PROPERTIES LINK_DEPENDS "${SHADER_FILES}")

target_include_directories(tank-game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(tank-game PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include/external)

file(GLOB_RECURSE SHADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*"
)

# Emscripten-specific build flags
if(EMSCRIPTEN)
  target_compile_options(tank-game PRIVATE
        -O0
        -Wall
        -g2
        -sNO_DISABLE_EXCEPTION_CATCHING
    )

  target_link_options(tank-game PRIVATE
        "-sUSE_GLFW=3"
        "-sFULL_ES3=1"
        "-sMIN_WEBGL_VERSION=2"
        "-sMAX_WEBGL_VERSION=2"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "--embed-file" "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders@/shaders"
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/template.html"
        -g2
        -sSAFE_HEAP=1
        -sASSERTIONS=2
        -sNO_DISABLE_EXCEPTION_CATCHING


    )

endif()


