#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <tank-game/InputHelper.hpp>
#include "tank-game/Lighting.hpp"
#include "tank-game/TimingData.hpp"
#include "tank-game/gameObjects/Bullet.hpp"
using namespace Tnk;
void Game::startGame() {
  maze.makeMaze(6);
  getMazeLines();
  Rect tankBounds = {
      .center = {-0.9, -0.9}, .size = {0.11, 0.075}, .rotation = 0};
  playerTank = Tank(tankBounds);
  playerTank.light.location = playerTank.bounds.center;
  playerTank.sprite.texture = "playerTank";
  mazeSize = maze.mazeSize;
}

std::array<PointLight, MAX_LIGHTS> Game::getLights() {
  std::array<PointLight, MAX_LIGHTS> lights;
  numLights = 1;
  lights[0] = playerTank.light;
  for (int i = 0; i < bulletVec.size(); i++) {
    numLights++;
    lights[i + 1] = bulletVec[i].light;
  }

  std::cout << numLights << std::endl;
  return lights;
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
    if (b.bounces > b.maxBounces) {
      bulletVec.erase(bulletVec.begin() + i);
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
