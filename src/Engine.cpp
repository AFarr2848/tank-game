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
  lighting = std::make_unique<Lighting>();
  renderer = std::make_unique<Renderer>(*win, *shaderMan, *bufferMan, *texMan,
                                        *lighting);
  game = std::make_unique<Game>(*inputHelper, *timingData);

  std::cout << "Window init..." << std::endl;
  win->init();
  std::cout << "Load shaders..." << std::endl;
  shaderMan->loadShaders();
  std::cout << "Renderer init..." << std::endl;
  std::cout << "Starting game..." << std::endl;
  game->startGame();
  std::cout << "Making buffers..." << std::endl;
  bufferMan->makeBuffers();
  std::cout << "Making textures..." << std::endl;
  texMan->makeTextures(win->width, win->height);
  texMan->makeLightingTextures(glm::vec2(win->width, win->height) *
                               glm::vec2(lighting->lightingResolution));
  texMan->makeWallTex(game->maze);

  bufferMan->updateBuffer("lineBuffer", game->mazeLines.data(),
                          sizeof(glm::vec2) * game->mazeLines.size());
  lighting->initLocations(shaderMan->shaderProgramMap["cascadeProgram"],
                          shaderMan->shaderProgramMap["mergeProgram"],
                          shaderMan->shaderProgramMap["mazeBakeProgram"],
                          shaderMan->shaderProgramMap["averageProgram"]);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  lighting->bakeInfo = {.mazeSize = game->mazeSize,
                        .screenSize = {win->width, win->height},
                        .numCascades = MAX_CASCADES};
  renderer->bakeMazeCollisions();

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
  if (win->resized) {
    screenResized();
    win->resized = false;
  }

  timingData->incrementTiming(emscripten_get_now());
  glfwPollEvents();
  inputHelper->updateInputs();
  game->updateGame();
  updateDrawUniforms();

  glBindFramebuffer(GL_FRAMEBUFFER, texMan->fbo);
  renderer->drawMaze(game->mazeLines);

  auto lights = game->getLights();
  renderer->drawCascades(lights);
  renderer->mergeCascades();

  auto sprites = game->getSprites();
  renderer->drawSprites(sprites);

  renderer->combineLighting();

  unsigned char px[4];
  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);  // forces a sync

  glfwSwapBuffers(win->window);
}

Engine::~Engine() = default;
Engine::Engine() = default;

void Engine::screenResized() {
  texMan->makeBufferTextures(win->width, win->height);
  texMan->makeLightingTextures(glm::vec2(win->width, win->height) *
                               glm::vec2(lighting->lightingResolution));

  lighting->bakeInfo.screenSize = {win->width, win->height};
  renderer->bakeMazeCollisions();
}

void Engine::updateDrawUniforms() {
  lighting->cascadeInfo.screenSize = {win->width, win->height};
  lighting->mergeInfo.screenSize = {win->width, win->height};
  lighting->cascadeInfo.numLights = game->numLights;
  lighting->cascadeInfo.mazeSize = game->mazeSize;
}

void Engine::changeLightingResolution(float newResolution) {
  lighting->lightingResolution = newResolution;
  texMan->makeLightingTextures(glm::vec2(win->width, win->height) *
                               glm::vec2(lighting->lightingResolution));
  renderer->bakeMazeCollisions();
}
