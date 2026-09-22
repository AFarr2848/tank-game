#include "tank-game/Engine.hpp"
#include <memory>
#include "emscripten/emscripten.h"
#include "tank-game/Renderer.hpp"
#include "tank-game/Window.hpp"
#include "tank-game/managers/ShaderManager.hpp"
using namespace Tnk;

void Engine::startEngine() {
  win = std::make_unique<Window>();
  shaderMan = std::make_unique<ShaderManager>();
  renderer = std::make_unique<Renderer>(*win, *shaderMan);

  win->init();
  shaderMan->loadShaders();

  emscripten_set_main_loop_arg(mainLoop, this, 0, 1);

  glfwDestroyWindow(win->window);
  glfwTerminate();
}

void Engine::mainLoop(void* arg) {
  Engine* engine = static_cast<Engine*>(arg);
  engine->update();
}

void Engine::update() {
  glfwPollEvents();
  renderer->drawScreen();
  glfwSwapBuffers(win->window);
}

Engine::~Engine() = default;
Engine::Engine() = default;
