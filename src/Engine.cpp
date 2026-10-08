#include "tank-game/Engine.hpp"
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <ostream>
#include "emscripten/emscripten.h"
#include "tank-game/Game.hpp"
#include "tank-game/InputHelper.hpp"
#include "tank-game/Renderer.hpp"
#include "tank-game/TimingData.hpp"
#include "tank-game/Window.hpp"
#include "tank-game/managers/BufferManager.hpp"
#include "tank-game/managers/ShaderManager.hpp"
#include "tank-game/managers/TextureManager.hpp"
using namespace Tnk;

void Engine::startEngine() {
  timingData = std::make_unique<TimingData>();
  inputHelper = std::make_unique<InputHelper>();
  win = std::make_unique<Window>(*inputHelper);
  shaderMan = std::make_unique<ShaderManager>();
  bufferMan = std::make_unique<BufferManager>();
  texMan = std::make_unique<TextureManager>();
  renderer = std::make_unique<Renderer>(*win, *shaderMan, *bufferMan, *texMan);
  game = std::make_unique<Game>(*inputHelper, *timingData);

  win->init();
  shaderMan->loadShaders();
  std::cout << "Starting game..." << std::endl;
  game->startGame();
  std::cout << "Starting main loop..." << std::endl;
  bufferMan->makeBuffers();
  texMan->makeTextures();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
  timingData->deltaTime = glfwGetTime() - timingData->lastTime;
  timingData->lastTime = glfwGetTime();

  inputHelper->updateInputs();
  game->updateGame();

  bufferMan->updateBuffer("lineBuffer", game->mazeLines);

  glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  renderer->drawMaze(game->mazeLines);
  auto sprites = game->getSprites();
  renderer->drawSprites(sprites);

  glfwSwapBuffers(win->window);
}

Engine::~Engine() = default;
Engine::Engine() = default;
