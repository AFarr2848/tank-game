#include "tank-game/Engine.hpp"
#include <memory>
#include "emscripten/emscripten.h"
#include "tank-game/Renderer.hpp"
#include "tank-game/Window.hpp"
#include "tank-game/managers/ShaderManager.hpp"

void tnk_Engine::startEngine() {
  win = std::make_unique<tnk_Window>();
  shaderMan = std::make_unique<tnk_ShaderManager>();
  renderer = std::make_unique<tnk_Renderer>(*win, *shaderMan);

  win->init();
  shaderMan->loadShaders();

  emscripten_set_main_loop_arg(mainLoop, this, 0, 1);

  glfwDestroyWindow(win->window);
  glfwTerminate();
}

void tnk_Engine::mainLoop(void* arg) {
  tnk_Engine* engine = static_cast<tnk_Engine*>(arg);
  engine->update();
}

void tnk_Engine::update() {
  glfwPollEvents();
  renderer->drawScreen();
  glfwSwapBuffers(win->window);
}

tnk_Engine::~tnk_Engine() = default;
tnk_Engine::tnk_Engine() = default;
