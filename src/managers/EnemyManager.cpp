#include "tank-game/managers/EnemyManager.hpp"
#include <algorithm>
#include <cstddef>
#include <queue>
#include "tank-game/Game.hpp"
#include "tank-game/gameObjects/Maze.hpp"
#include "tank-game/gameObjects/PlayerTank.hpp"

using namespace Tnk;

void EnemyManager::updateEnemies(PlayerTank& playerTank,
                                 Maze& maze,
                                 std::vector<Bullet>& bulletVec,
                                 float deltaTime) {
  for (auto& tnk : enemyTanks) {
    tnk.update(deltaTime);
    if (tnk.requestShoot) {
      tnk.shoot(bulletVec);
    }

    updateEnemyAI(tnk, playerTank, maze);
  }
  trySpawnTank(playerTank, maze);
}

void EnemyManager::updateEnemyAI(EnemyTank& tank,
                                 PlayerTank& playerTank,
                                 Maze& maze) {
  if (tank.tankAI.thinkTimer > AI_THINK_INTERVAL) {
    tank.tankAI.thinkTimer = 0;
    glm::vec2 targetDiff = tank.bounds.center - tank.tankAI.moveTarget;
    if (hypot(targetDiff.x, targetDiff.y) < 0.1 ||
        tank.tankAI.moveTarget == glm::vec2(-1)) {
      auto path =
          findEnemyPath(maze.NDCtoMazeCoords(tank.bounds.center),
                        maze.NDCtoMazeCoords(playerTank.bounds.center), maze);
      if (path.size() > 1)
        tank.tankAI.moveTarget = path[1];
    }
  }
}

void EnemyManager::trySpawnTank(PlayerTank& playerTank, Maze& maze) {
  if (getTotalTankValues() < maxTankValue) {
    Rect bounds = {.center = getSpawnLocation(playerTank, maze),
                   .size = {0.11, 0.075},
                   .rotation = 0};
    EnemyTankCreateInfo createInfo;
    createInfo.tankValue = 1;
    createInfo.maxRicochets = 1;
    createInfo.texture = "playerTank";
    createInfo.bounds = bounds;

    enemyTanks.push_back(EnemyTank(createInfo));
  }
}

glm::vec2 EnemyManager::getSpawnLocation(PlayerTank& playerTank, Maze& maze) {
  std::random_device rd;

  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(0, maze.mazeSize - 1);
  glm::ivec2 playerLoc = maze.mazeToNdcCoords(playerTank.bounds.center);
  for (int i = 0; i < 100; i++) {
    glm::ivec2 loc = {distrib(gen), distrib(gen)};
    if (!(abs(playerLoc.x - loc.x) <= 1 && abs(playerLoc.y - loc.y) <= 1)) {
      return maze.mazeToNdcCoords(loc);
    }
  }
  throw std::runtime_error("oopies tanks spawn failed oops");
}

Tank* EnemyManager::getEnemyByID(uint32_t id) {
  for (auto& t : enemyTanks) {
    if (t.id == id)
      return &t;
  }
  return nullptr;
}

void EnemyManager::killTank(uint32_t id) {
  Tank* t = getEnemyByID(id);
  if (t != nullptr)
    std::erase(enemyTanks, *t);
}

int EnemyManager::getTotalTankValues() {
  int sum = 0;
  for (EnemyTank& t : enemyTanks) {
    sum += t.tankValue;
  }
  return sum;
}

std::vector<glm::vec2> EnemyManager::findEnemyPath(glm::ivec2 start,
                                                   glm::ivec2 end,
                                                   Maze& maze) {
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
    path.push_back(maze.mazeToNdcCoords(allNodes[curr].coord));
    curr = allNodes[curr].parentIndex;
  }

  std::ranges::reverse(path);
  return path;
}

void EnemyManager::updateEnemyShots(PlayerTank& playerTank, Maze& maze) {
  for (auto& t : enemyTanks)
    t.tankAI.hasShot = false;

  const int NUM_RAYS = 360;
  for (int i = 0; i < NUM_RAYS; i++) {
    bool foundCol = false;
    ShotRay ray = {
        playerTank.bounds.center,
        static_cast<float>(std::numbers::pi * 2 * (i / float(NUM_RAYS))), 0

    };

    while (!foundCol && ray.colNum < 2) {
      ShotRay nextRay = calculateRayBounce(ray, maze);
      for (auto& t : enemyTanks) {
        if (t.bounds.intersectRay(ray.pos, nextRay.pos)) {
          t.tankAI.hasShot = true;
          t.tankAI.shotAngle = ray.angle >= std::numbers::pi
                                   ? ray.angle - std::numbers::pi
                                   : ray.angle + std::numbers::pi;
          foundCol = true;
          break;
        }
      }
      ray = nextRay;
    }
  }
}

EnemyManager::ShotRay EnemyManager::calculateRayBounce(ShotRay ray,
                                                       Maze& maze) {
  // 1. Convert NDC input position to internal maze grid coordinates [0,
  // mazeSize]
  float gridX = (ray.pos.x + 1.0f) * 0.5f * maze.mazeSize;
  float gridY = (ray.pos.y + 1.0f) * 0.5f * maze.mazeSize;

  glm::vec2 dir = {std::cos(ray.angle), std::sin(ray.angle)};

  int mapX = static_cast<int>(std::floor(gridX));
  int mapY = static_cast<int>(std::floor(gridY));

  // Clamp to valid grid indices to prevent out-of-bounds index faults
  mapX = std::clamp(mapX, 0, maze.mazeSize - 1);
  mapY = std::clamp(mapY, 0, maze.mazeSize - 1);

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

  float t = 0.0f;
  while (!hitWall) {
    const auto& cell = maze.cells[mapY][mapX];  // cell we're leaving

    if (sideDist.x < sideDist.y) {
      if (stepX > 0 ? cell.wallEast : cell.wallWest) {
        hitWall = true;
        t = sideDist.x;
        normal = {-float(stepX), 0.0f};
        break;
      }
      sideDist.x += deltaDist.x;
      mapX += stepX;
    } else {
      if (stepY > 0 ? cell.wallSouth : cell.wallNorth) {
        hitWall = true;
        t = sideDist.y;
        normal = {0.0f, -float(stepY)};
        break;
      }
      sideDist.y += deltaDist.y;
      mapY += stepY;
    }
  }

  glm::vec2 hitGridPos = glm::vec2(gridX, gridY) + dir * t;

  // Nudge slightly away from the wall to prevent self-intersection bugs
  glm::vec2 offsetGridPos = hitGridPos + (normal * 0.001f);

  // 5. Calculate reflection angle: R = D - 2(D · N)N
  glm::vec2 reflectDir = dir - 2.0f * glm::dot(dir, normal) * normal;
  float newAngle = std::atan2(reflectDir.y, reflectDir.x);

  // 6. Convert grid space hit position back into NDC space [-1, 1]
  float ndcX = (offsetGridPos.x / maze.mazeSize) * 2.0f - 1.0f;
  float ndcY = (offsetGridPos.y / maze.mazeSize) * 2.0f - 1.0f;

  return ShotRay{
      .pos = {ndcX, ndcY}, .angle = newAngle, .colNum = ray.colNum + 1};
}
