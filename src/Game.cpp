#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <tank-game/InputHelper.hpp>
#include "tank-game/TimingData.hpp"
using namespace Tnk;
void Game::startGame() {
  maze.makeMaze(6);
  getMazeLines();
  Rect tankBounds = {
      .center = {-0.9, -0.9}, .size = {0.11, 0.075}, .rotation = 0};
  playerTank = Tank(tankBounds);
  playerTank.sprite.texture = "playerTank";
}

void Game::getMazeLines() {
  mazeLines = maze.makeLines();
}

void Game::getInputs() {
  if (this->inputHelper.isKeyDown(GLFW_KEY_W))
    playerTank.moveTank(timingData.deltaTime * 0.4);
  if (this->inputHelper.isKeyDown(GLFW_KEY_S))
    playerTank.moveTank(-timingData.deltaTime * 0.4);
  if (this->inputHelper.isKeyDown(GLFW_KEY_A))
    playerTank.turnTank(timingData.deltaTime * 2);
  if (this->inputHelper.isKeyDown(GLFW_KEY_D))
    playerTank.turnTank(-timingData.deltaTime * 2);
}

void Game::movePlayer() {}
