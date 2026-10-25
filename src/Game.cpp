#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <stdexcept>
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
  int currentLightIndex = 0;

  lights[currentLightIndex++] = playerTank.light;

  for (const auto& bullet : bulletVec) {
    if (currentLightIndex < MAX_LIGHTS) {
      lights[currentLightIndex++] = bullet.light;
    }
  }

  for (const auto& enemy : enemyTanks) {
    if (currentLightIndex < MAX_LIGHTS) {
      lights[currentLightIndex++] = enemy.light;
    }
  }

  numLights = currentLightIndex;
  return lights;
}
void Game::trySpawnTank() {
  std::cout << getTotalTankValues() << std::endl;
  if (getTotalTankValues() < maxTankValue) {
    Rect bounds = {
        .center = getSpawnLocation(), .size = {0.11, 0.075}, .rotation = 0};
    std::cout << "Tank spawned!" << std::endl;
    std::cout << glm::to_string(bounds.center) << std::endl;
    enemyTanks.push_back(
        EnemyTank(bounds, "playerTank", glm::vec3(7.0, 0.0, 0.0)));
  }
  std::cout << enemyTanks.size() << std::endl;
}

glm::vec2 Game::getSpawnLocation() {
  std::random_device rd;

  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(1, mazeSize - 1);
  glm::vec2 playerLoc = playerTank.bounds.center / glm::vec2(mazeSize);
  for (int i = 0; i < 10; i++) {
    glm::vec2 loc = {distrib(gen), distrib(gen)};
    if (abs(playerLoc.x - loc.x) > 2 && abs(playerLoc.y - loc.y) > 2) {
      return ((loc / glm::vec2(mazeSize)) - glm::vec2(0.5)) * glm::vec2(2.0) +
             glm::vec2(1.0 / mazeSize);
    }
  }
  throw std::runtime_error("oopies tanks spawn failed oops");
}

int Game::getTotalTankValues() {
  int sum = 0;
  for (EnemyTank& t : enemyTanks) {
    sum += t.tankValue;
  }
  return sum;
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
  collideBullets();
  trySpawnTank();
  getInputs();
}

void Game::collideBullets() {
  for (auto& b : bulletVec) {
    for (auto& t : enemyTanks) {
      if (t.bounds.contains(b.bounds.center)) {
        std::erase(enemyTanks, t);
      }
    }
  }
}

std::vector<Sprite> Game::getSprites() {
  std::vector<Sprite> sprites;
  playerTank.sprite.bounds = playerTank.bounds;
  sprites.push_back(playerTank.sprite);
  for (Bullet b : bulletVec) {
    b.sprite.bounds = b.bounds;
    sprites.push_back(b.sprite);
  }
  for (EnemyTank& t : enemyTanks) {
    t.sprite.bounds = t.bounds;
    sprites.push_back(t.sprite);
  }

  return sprites;
}
