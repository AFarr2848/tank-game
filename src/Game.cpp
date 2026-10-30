#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <queue>
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

glm::ivec2 Game::getTankMazeCoords(Tank& tank) {
  float norm_x = (tank.bounds.center.x + 1.0f) * 0.5f;
  float norm_y = (tank.bounds.center.y + 1.0f) * 0.5f;  // was (1 - y)
  int col = std::clamp(static_cast<int>(norm_x * mazeSize), 0, mazeSize - 1);
  int row = std::clamp(static_cast<int>(norm_y * mazeSize), 0, mazeSize - 1);
  return {row, col};
}

glm::vec2 Game::mazeToNdcCoords(glm::ivec2 mazeCoords) {
  int row = mazeCoords.x;
  int col = mazeCoords.y;
  float x = ((col + 0.5f) / mazeSize) * 2.0f - 1.0f;
  float y = ((row + 0.5f) / mazeSize) * 2.0f - 1.0f;  // was 1 - 2*norm_y
  return {x, y};
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
}

glm::vec2 Game::getSpawnLocation() {
  std::random_device rd;

  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(0, mazeSize - 1);
  glm::ivec2 playerLoc = getTankMazeCoords(playerTank);
  for (int i = 0; i < 100; i++) {
    glm::ivec2 loc = {distrib(gen), distrib(gen)};
    if (!(abs(playerLoc.x - loc.x) <= 1 && abs(playerLoc.y - loc.y) <= 1)) {
      return mazeToNdcCoords(loc);
    }
  }
  throw std::runtime_error("oopies tanks spawn failed oops");
}

std::vector<glm::ivec2> Game::findMazePath(glm::ivec2 start, glm::ivec2 end) {
  struct Node {
    glm::ivec2 coord;
    int parentIndex;
  };

  std::vector<Node> allNodes;
  std::queue<int> queue;
  std::vector<glm::ivec2> explored;

  allNodes.push_back({.coord = start, .parentIndex = -1});
  queue.push(0);
  explored.push_back(start);

  int endIndex = -1;

  while (!queue.empty()) {
    int currentIndex = queue.front();
    queue.pop();

    if (allNodes[currentIndex].coord == end) {
      endIndex = currentIndex;
      break;
    }

    std::vector<glm::ivec2> dirs =
        maze.getConnectedCells(allNodes[currentIndex].coord);
    for (const auto& dir : dirs) {
      bool inExplored = false;
      for (const auto& exp : explored) {
        if (exp == dir) {
          inExplored = true;
          break;
        }
      }

      if (!inExplored) {
        explored.push_back(dir);
        allNodes.push_back({.coord = dir, .parentIndex = currentIndex});
        queue.push(allNodes.size() - 1);
      }
    }
  }

  if (endIndex == -1) {
    return {};
  }

  std::vector<glm::ivec2> path;
  int curr = endIndex;
  while (curr != -1) {
    path.push_back(allNodes[curr].coord);
    curr = allNodes[curr].parentIndex;
  }

  std::ranges::reverse(path);
  return path;
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
