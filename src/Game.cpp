#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
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
  auto isKeyDown = [this](int key) { return this->inputHelper.isKeyDown(key); };

  if (isKeyDown(GLFW_KEY_W))
    playerTank.moveTank(
        playerTank.checkMoveCollision(timingData.deltaTime * 0.4, maze));
  if (isKeyDown(GLFW_KEY_S))
    playerTank.moveTank(
        playerTank.checkMoveCollision(-timingData.deltaTime * 0.4, maze));
  if (isKeyDown(GLFW_KEY_A))
    playerTank.turnTank(timingData.deltaTime * 2);
  if (isKeyDown(GLFW_KEY_D))
    playerTank.turnTank(-timingData.deltaTime * 2);
  if (this->inputHelper.isKeyDownToggle(GLFW_KEY_SPACE))
    playerTank.shoot(bulletVec);
}

void Game::updateGame() {
  for (int i = 0; i < bulletVec.size(); i++) {
    Bullet& b = bulletVec.at(i);
    b.moveBullet(timingData.deltaTime, maze);
    std::cout << b.bounces << std::endl;
    if (b.bounces > b.maxBounces) {
      bulletVec.erase(bulletVec.begin() + i);
      std::cout << "erased" << std::endl;
    }
  }
  getInputs();
}

std::vector<Sprite> Game::getSprites() {
  std::vector<Sprite> sprites;
  playerTank.sprite.bounds = playerTank.bounds;
  sprites.push_back(playerTank.sprite);
  for (Bullet b : bulletVec) {
    b.sprite.bounds = b.bounds;
    sprites.push_back(b.sprite);
  }

  return sprites;
}
