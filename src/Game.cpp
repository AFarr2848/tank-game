#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <tank-game/InputHelper.hpp>
#include "tank-game/Lighting.hpp"
#include "tank-game/TimingData.hpp"
#include "tank-game/gameObjects/Bullet.hpp"
#include "tank-game/gameObjects/EnemyTank.hpp"
#include "tank-game/gameObjects/PlayerTank.hpp"
#include "tank-game/gameObjects/Tank.hpp"
using namespace Tnk;
void Game::startGame() {
  maze.makeMaze(6);
  getMazeLines();
  PlayerCreateInfo playerInfo;
  playerInfo.bounds = {
      .center = {-0.9, -0.9}, .size = {0.11, 0.075}, .rotation = 0};
  playerInfo.texture = "playerTank";
  playerInfo.lightColor = glm::vec3(10.0f);
  playerInfo.moveSpeed = 0.15;
  playerInfo.turnSpeed = 0.07;

  playerTank = PlayerTank(playerInfo);
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
  if (getTotalTankValues() < maxTankValue) {
    Rect bounds = {
        .center = getSpawnLocation(), .size = {0.11, 0.075}, .rotation = 0};
    std::cout << "Tank spawned!" << std::endl;
    std::cout << glm::to_string(bounds.center) << std::endl;
    EnemyTankCreateInfo createInfo;
    createInfo.tankValue = 1;
    createInfo.maxRicochets = 1;
    createInfo.texture = "playerTank";
    createInfo.bounds = bounds;

    enemyTanks.push_back(EnemyTank(createInfo));
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

void Game::updateTankAI(EnemyTank& tank) {
  if (tank.tankAI.thinkTimer > AI_THINK_INTERVAL) {
    tank.tankAI.thinkTimer = 0;
    glm::vec2 targetDiff = tank.bounds.center - tank.tankAI.moveTarget;
    if (hypot(targetDiff.x, targetDiff.y) < 0.1 ||
        tank.tankAI.moveTarget == glm::vec2(-1)) {
      auto path =
          findMazePath(getTankMazeCoords(tank), getTankMazeCoords(playerTank));
      if (path.size() > 1)
        tank.tankAI.moveTarget = path[1];
    }
  }
}

// tbd
void Game::checkAvailableShots() {
  for (int i = 0; i < 64; i++) {
    bool foundCol = false;
    ShotRay ray = {playerTank.bounds.center,
                   static_cast<float>(std::numbers::pi * 2 * (i / 64.0)), 0

    };
    while (!foundCol && ray.colNum < 1) {
      ShotRay nextRay = calculateRayBounce(ray);
      for (auto& t : enemyTanks) {
        if (t.bounds.intersectRay(ray.pos, nextRay.pos)) {
          // tank collides with ray
          foundCol = true;
          std::cout << "Shot found!" << std::endl;
        }
      }
      ray = nextRay;
    }
  }
}

Game::ShotRay Game::calculateRayBounce(ShotRay ray) {
  // 1. Convert NDC input position to internal maze grid coordinates [0,
  // mazeSize]
  float gridX = (ray.pos.x + 1.0f) * 0.5f * mazeSize;
  float gridY = (ray.pos.y + 1.0f) * 0.5f * mazeSize;

  glm::vec2 dir = {std::cos(ray.angle), std::sin(ray.angle)};

  int mapX = static_cast<int>(std::floor(gridX));
  int mapY = static_cast<int>(std::floor(gridY));

  // Clamp to valid grid indices to prevent out-of-bounds index faults
  mapX = std::clamp(mapX, 0, mazeSize - 1);
  mapY = std::clamp(mapY, 0, mazeSize - 1);

  // 2. DDA Step setup
  glm::vec2 deltaDist = {(dir.x == 0) ? 1e30f : std::abs(1.0f / dir.x),
                         (dir.y == 0) ? 1e30f : std::abs(1.0f / dir.y)};

  int stepX = (dir.x < 0) ? -1 : 1;
  int stepY = (dir.y < 0) ? -1 : 1;

  glm::vec2 sideDist;
  sideDist.x = (dir.x < 0) ? (gridX - mapX) * deltaDist.x
                           : (mapX + 1.0f - gridX) * deltaDist.x;
  sideDist.y = (dir.y < 0) ? (gridY - mapY) * deltaDist.y
                           : (mapY + 1.0f - gridY) * deltaDist.y;

  bool hitWall = false;
  int hitSide = 0;  // 0 = X-axis wall, 1 = Y-axis wall
  glm::vec2 normal = {0.0f, 0.0f};

  // 3. Step through grid cells until an inner wall or outer boundary is hit
  while (!hitWall) {
    if (sideDist.x < sideDist.y) {
      sideDist.x += deltaDist.x;
      mapX += stepX;
      hitSide = 0;
    } else {
      sideDist.y += deltaDist.y;
      mapY += stepY;
      hitSide = 1;
    }

    // Check outer maze boundaries (treat outer edges as solid walls)
    if (mapX < 0 || mapX >= mazeSize || mapY < 0 || mapY >= mazeSize) {
      hitWall = true;
      if (hitSide == 0) {
        normal = {(stepX > 0) ? -1.0f : 1.0f, 0.0f};
        mapX = std::clamp(mapX, 0, mazeSize - 1);
      } else {
        normal = {0.0f, (stepY > 0) ? -1.0f : 1.0f};
        mapY = std::clamp(mapY, 0, mazeSize - 1);
      }
      break;
    }

    // Check internal maze walls
    const auto& cell = maze.cells[mapY][mapX];
    if (hitSide == 0) {
      if (stepX > 0 && cell.wallEast) {
        hitWall = true;
        normal = {-1.0f, 0.0f};
      } else if (stepX < 0 && cell.wallWest) {
        hitWall = true;
        normal = {1.0f, 0.0f};
      }
    } else {
      if (stepY > 0 && cell.wallSouth) {
        hitWall = true;
        normal = {0.0f, -1.0f};
      } else if (stepY < 0 && cell.wallNorth) {
        hitWall = true;
        normal = {0.0f, 1.0f};
      }
    }
  }

  // 4. Calculate exact hit coordinate in grid space
  float perpWallDist =
      (hitSide == 0) ? (sideDist.x - deltaDist.x) : (sideDist.y - deltaDist.y);
  glm::vec2 hitGridPos = glm::vec2(gridX, gridY) + dir * perpWallDist;

  // Nudge slightly away from the wall to prevent self-intersection bugs
  glm::vec2 offsetGridPos = hitGridPos + (normal * 0.001f);

  // 5. Calculate reflection angle: R = D - 2(D · N)N
  glm::vec2 reflectDir = dir - 2.0f * glm::dot(dir, normal) * normal;
  float newAngle = std::atan2(reflectDir.y, reflectDir.x);

  // 6. Convert grid space hit position back into NDC space [-1, 1]
  float ndcX = (offsetGridPos.x / mazeSize) * 2.0f - 1.0f;
  float ndcY = (offsetGridPos.y / mazeSize) * 2.0f - 1.0f;

  return ShotRay{
      .pos = {ndcX, ndcY}, .angle = newAngle, .colNum = ray.colNum + 1};
}

std::vector<glm::vec2> Game::findMazePath(glm::ivec2 start, glm::ivec2 end) {
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

  std::vector<glm::vec2> path;
  int curr = endIndex;
  while (curr != -1) {
    path.push_back(mazeToNdcCoords(allNodes[curr].coord));
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
    playerTank.moveTank(playerTank.checkMoveCollision(
        timingData.deltaTime * playerTank.moveSpeed, maze));
  if (isKeyDown(GLFW_KEY_S))
    playerTank.moveTank(playerTank.checkMoveCollision(
        -timingData.deltaTime * playerTank.moveSpeed, maze));
  if (isKeyDown(GLFW_KEY_A))
    playerTank.turnTank(timingData.deltaTime * playerTank.turnSpeed);
  if (isKeyDown(GLFW_KEY_D))
    playerTank.turnTank(-timingData.deltaTime * playerTank.turnSpeed);
  if (this->inputHelper.isKeyDownToggle(GLFW_KEY_SPACE))
    playerTank.shoot(bulletVec);
}

void Game::updateGame() {
  playerTank.update(timingData.deltaTime);
  if (playerTank.mathTimer > AI_THINK_INTERVAL) {
    playerTank.mathTimer = 0;
    checkAvailableShots();
  }
  for (auto& tnk : enemyTanks) {
    tnk.update(timingData.deltaTime);
    updateTankAI(tnk);
  }
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
