#include "tank-game/Engine.hpp"
#include <GLES3/gl3.h>
#include <iostream>
#include <memory>
#include <ostream>
#include "emscripten/emscripten.h"
#include "tank-game/Game.hpp"
#include "tank-game/Geometry.hpp"
#include "tank-game/Renderer.hpp"
#include "tank-game/Window.hpp"
#include "tank-game/gameObjects/Sprite.hpp"
#include "tank-game/managers/BufferManager.hpp"
#include "tank-game/managers/ShaderManager.hpp"
#include "tank-game/managers/TextureManager.hpp"
using namespace Tnk;

void Engine::startEngine() {
  win = std::make_unique<Window>();
  shaderMan = std::make_unique<ShaderManager>();
  bufferMan = std::make_unique<BufferManager>();
  texMan = std::make_unique<TextureManager>();
  renderer = std::make_unique<Renderer>(*win, *shaderMan, *bufferMan, *texMan);
  game = std::make_unique<Game>();

  win->init();
  shaderMan->loadShaders();
  std::cout << "Starting game..." << std::endl;
  game->startGame();
  std::cout << "Starting main loop..." << std::endl;
  bufferMan->makeBuffers();
  texMan->makeTextures();

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
  bufferMan->updateBuffer("lineBuffer", game->mazeLines);
  std::cout << game->mazeLines.size() << std::endl;
  // renderer->drawScreen();
  renderer->drawMaze(game->mazeLines);
  renderer->drawSprites({Sprite{}});

  glfwSwapBuffers(win->window);
}

Engine::~Engine() = default;
Engine::Engine() = default;
