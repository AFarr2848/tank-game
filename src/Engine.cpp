#include "tank-game/Engine.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <numbers>
#include <ostream>
#include "emscripten/emscripten.h"
#include "tank-game/Game.hpp"
#include "tank-game/InputHelper.hpp"
#include "tank-game/Lighting.hpp"
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

  std::cout << "Window init..." << std::endl;
  win->init();
  std::cout << "Load shaders..." << std::endl;
  shaderMan->loadShaders();
  std::cout << "Renderer init..." << std::endl;
  renderer->init(shaderMan->shaderProgramMap["cascadeProgram"],
                 shaderMan->shaderProgramMap["averageProgram"],
                 shaderMan->shaderProgramMap["mergeProgram"]);
  std::cout << "Starting game..." << std::endl;
  game->startGame();
  std::cout << "Making buffers..." << std::endl;
  bufferMan->makeBuffers();
  std::cout << "Making textures..." << std::endl;
  texMan->makeTextures(win->width, win->height);
  texMan->makeWallTex(game->maze);

  bufferMan->updateBuffer("lineBuffer", game->mazeLines.data(),
                          sizeof(glm::vec2) * game->mazeLines.size());

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);

  std::cout << "Starting main loop..." << std::endl;
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
  if (win->resized) {
    texMan->makeBufferTextures(win->width, win->height);
    win->resized = false;
  }

  inputHelper->updateInputs();
  game->updateGame();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindFramebuffer(GL_FRAMEBUFFER, texMan->fbo);
  renderer->drawMaze(game->mazeLines);

  glDisable(GL_BLEND);
  CascadeInfo cascadeInfo = {
      .mazeSize = game->mazeSize,
      .screenSize = glm::vec2(win->width, win->height),
      .cascadeLevel = 0,
      .linearInterval = 2,
      .rayInterval = std::numbers::pi / 2,
      .backgroundColor = glm::vec3(0.2f, 0.2f, 0.25f),
      .numLights = game->numLights,

  };
  auto lights = game->getLights();
  renderer->drawCascades(lights, cascadeInfo);
  renderer->mergeCascades(cascadeInfo);

  auto sprites = game->getSprites();
  renderer->drawSprites(sprites);

  renderer->combineLighting();

  glfwSwapBuffers(win->window);
}

Engine::~Engine() = default;
Engine::Engine() = default;
