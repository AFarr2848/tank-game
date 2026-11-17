#This file was made by clade bc I hate cmake
cmake_minimum_required(VERSION 3.15)
project(tank-game CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)
set(IMGUI_BACKEND_SOURCES
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)


# Source files
set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Maze.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/InputHelper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/ShaderManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/BufferManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/TextureManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/EnemyManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gameObjects/Bullet.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gameObjects/SpecialTanks.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gameObjects/EnemyTank.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gameObjects/PlayerTank.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Geometry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Lighting.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI.cpp
)

# Output executable target
add_executable(tank-game ${SOURCES})

# Target file extension must be .html for Emscripten
set_target_properties(tank-game PROPERTIES
    OUTPUT_NAME "index"   # Automatically outputs index.html directly
    SUFFIX ".html"
)

target_compile_definitions(tank-game PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Track shaders for CMake rebuilds
file(GLOB_RECURSE SHADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*"
)
set_target_properties(tank-game PROPERTIES LINK_DEPENDS "${SHADER_FILES}")

target_include_directories(tank-game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(tank-game PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include/external
${CMAKE_CURRENT_SOURCE_DIR}/include/external/imgui

)

target_link_libraries(tank-game imgui)

if(EMSCRIPTEN)
  # Base Emscripten link options
  target_link_options(tank-game PRIVATE
        "-sUSE_GLFW=3"
        "-sFULL_ES3=1"
        "-sMIN_WEBGL_VERSION=2"
        "-sMAX_WEBGL_VERSION=2"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "--embed-file=${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/@/shaders/"
        "--embed-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/@/textures/"
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/template.html"
  )

  # Release flags (optimized, small binaries)
  target_compile_options(tank-game PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Debug>:-O0 -g2 -Wall>
  )

  # Linker options dependent on build mode
  target_link_options(tank-game PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Debug>:-g2 -sSAFE_HEAP=1 -sASSERTIONS=2 -sNO_DISABLE_EXCEPTION_CATCHING>
  )
endif()

