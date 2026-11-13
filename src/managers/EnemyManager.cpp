#include "tank-game/managers/EnemyManager.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <numbers>
#include <queue>
#include "tank-game/Game.hpp"
#include "tank-game/Random.hpp"
#include "tank-game/gameObjects/EnemyTank.hpp"
#include "tank-game/gameObjects/Maze.hpp"
#include "tank-game/gameObjects/PlayerTank.hpp"
#include "tank-game/gameObjects/SpecialTanks.hpp"

using namespace Tnk;
const float PI = std::numbers::pi;

void EnemyManager::updateEnemies(PlayerTank& playerTank,
                                 Maze& maze,
                                 std::vector<Bullet>& bulletVec,
                                 float deltaTime) {
  for (auto& tnk : enemyTanks) {
    tnk->update(deltaTime);
    if (tnk->requestShoot) {
      tnk->shoot(bulletVec);
    }

    updateEnemyAI(*tnk, playerTank, maze);
  }
  trySpawnTank(playerTank, maze);
}

void EnemyManager::initEnemyTypes() {
  EnemyTankCreateInfo dorkInfo = {
      .tankValue = 2,
      .maxRicochets = 0,
      .shotTolerance = PI / 32,
  };
  dorkInfo.moveSpeed = 0.2;
  dorkInfo.turnSpeed = 1;
  dorkInfo.maxBullets = 1;
  dorkInfo.fireCooldown = 2;
  dorkInfo.texture = "enemyDork";
  enemyTypes["dork"] = dorkInfo;

  EnemyTankCreateInfo trooperInfo = {
      .tankValue = 4,
      .maxRicochets = 1,
      .shotTolerance = PI / 32,
  };
  trooperInfo.moveSpeed = 0.3;
  trooperInfo.turnSpeed = 1.5;
  trooperInfo.maxBullets = 2;
  trooperInfo.fireCooldown = 1;
  trooperInfo.texture = "enemyTrooper";
  enemyTypes["trooper"] = trooperInfo;

  EnemyTankCreateInfo sprayerInfo = {
      .tankValue = 7,
      .maxRicochets = 0,
      .shotTolerance = PI / 8,
  };
  sprayerInfo.moveSpeed = 0.1;
  sprayerInfo.turnSpeed = 1.5;
  sprayerInfo.maxBullets = 100;
  sprayerInfo.fireCooldown = 0.1;
  sprayerInfo.texture = "enemySprayer";
  enemyTypes["sprayer"] = sprayerInfo;

  EnemyTankCreateInfo sniperInfo = {
      .tankValue = 7,
      .maxRicochets = 3,
      .shotTolerance = PI / 48,
  };
  sniperInfo.moveSpeed = 0.1;
  sniperInfo.turnSpeed = 1.0;
  sniperInfo.maxBullets = 1;
  sniperInfo.fireCooldown = 4;
  sniperInfo.bulletSpeed = 1.5;
  sniperInfo.texture = "enemySniper";
  enemyTypes["sniper"] = sniperInfo;

  EnemyTankCreateInfo chaserInfo = {
      .tankValue = 5,
  };
  chaserInfo.moveSpeed = 0.5;
  chaserInfo.turnSpeed = 3.5;
  chaserInfo.texture = "enemyRam";
  chaserInfo.bounds.size *= 1.5;
  enemyTypes["chaser"] = chaserInfo;
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

std::string EnemyManager::getRandomTankName(int maxValue) {
  std::vector<std::string> names;
  for (const auto& [name, info] : enemyTypes) {
    if (info.tankValue <= maxValue)
      names.push_back(name);
  }
  if (names.empty())
    return "dork";

  int tankIndex = Random::getInt(0, int(names.size() - 1));

  return names[tankIndex];
}

void EnemyManager::trySpawnTank(PlayerTank& playerTank, Maze& maze) {
  if (nextTankSpawn == "") {
    int maxValue = maxTankValue / 2;
    nextTankSpawn = getRandomTankName(maxValue);
  }

  else if (getTotalTankValues() < maxTankValue) {
    auto createInfo = enemyTypes[nextTankSpawn];
    createInfo.bounds.center = getSpawnLocation(playerTank, maze);

    if (nextTankSpawn == "sprayer")
      enemyTanks.push_back(std::make_unique<EnemyTankSprayer>(createInfo));
    else if (nextTankSpawn == "chaser")
      enemyTanks.push_back(std::make_unique<EnemyTankChaser>(createInfo));
    else
      enemyTanks.push_back(std::make_unique<EnemyTank>(createInfo));
    nextTankSpawn = "";
  }
}

glm::vec2 EnemyManager::getSpawnLocation(PlayerTank& playerTank, Maze& maze) {
  glm::ivec2 playerLoc = maze.NDCtoMazeCoords(playerTank.bounds.center);
  for (int i = 0; i < 100; i++) {
    glm::ivec2 loc = {Random::getInt(0, maze.mazeSize - 1),
                      Random::getInt(0, maze.mazeSize - 1)};
    if (!(abs(playerLoc.x - loc.x) <= 1 && abs(playerLoc.y - loc.y) <= 1)) {
      return maze.mazeToNdcCoords(loc);
    }
  }
  throw std::runtime_error("oopies tanks spawn failed oops");
}

std::unique_ptr<EnemyTank>* EnemyManager::getEnemyByID(uint32_t id) {
  for (auto& t : enemyTanks) {
    if (t->id == id)
      return &t;
  }
  return nullptr;
}

void EnemyManager::killTank(uint32_t id) {
  auto& t = *getEnemyByID(id);
  if (t != nullptr) {
    maxTankValue += 1;
    std::erase(enemyTanks, t);
  }
}

int EnemyManager::getTotalTankValues() {
  int sum = 0;
  for (auto& t : enemyTanks) {
    sum += t->tankValue;
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
  for (auto& t : enemyTanks) {
    t->tankAI.hasShot = false;
  }

  const int NUM_RAYS = 360;
  for (int i = 0; i < NUM_RAYS; i++) {
    bool foundCol = false;
    ShotRay ray = {
        playerTank.bounds.center,
        static_cast<float>(std::numbers::pi * 2 * (i / float(NUM_RAYS))), 0

    };

    while (!foundCol && ray.colNum < 5) {
      ShotRay nextRay = calculateRayBounce(ray, maze);
      for (auto& t : enemyTanks) {
        if (ray.colNum <= t->maxRicochets &&
            t->bounds.intersectRay(ray.pos, nextRay.pos)) {
          if (t->tankAI.hasShot && t->tankAI.currentShotRicochets < ray.colNum)
            continue;
          float newShotAngle = atan2(sin(ray.angle + std::numbers::pi),
                                     cos(ray.angle + std::numbers::pi));
          if (t->tankAI.hasShot) {
            float angleDiff =
                atan2(std::sin(t->tankAI.shotAngle - t->bounds.rotation),
                      std::cos(t->tankAI.shotAngle - t->bounds.rotation));
            float newAngleDiff =
                atan2(std::sin(newShotAngle - t->bounds.rotation),
                      std::cos(newShotAngle - t->bounds.rotation));
            if (abs(angleDiff) <= abs(newShotAngle))
              continue;
          }
          if (verifyShot(*t, newShotAngle, playerTank, maze)) {
            t->tankAI.hasShot = true;
            t->tankAI.currentShotRicochets = ray.colNum;
            t->tankAI.shotAngle = newShotAngle;
            foundCol = true;
            break;
          }
        }
      }
      ray = nextRay;
    }
  }
}

bool EnemyManager::verifyShot(const EnemyTank& shooter,
                              float angle,
                              const PlayerTank& player,
                              Maze& maze) {
  const float bulletRadius = 0.03f;  // use your real bullet size / half-width

  glm::vec2 dir = {std::cos(angle), std::sin(angle)};
  glm::vec2 muzzle =
      shooter.bounds.center + dir * (shooter.bounds.size.x * 0.5f);
  glm::vec2 perp = {-dir.y, dir.x};

  int centerHitSegment = -1;

  for (float offset : {0.0f, -bulletRadius, bulletRadius}) {
    ShotRay ray = {muzzle + perp * offset, angle, 0};
    int hitSegment = -1;

    while (ray.colNum <= shooter.maxRicochets) {
      ShotRay next = calculateRayBounce(ray, maze);
      if (player.bounds.intersectRay(ray.pos, next.pos)) {
        hitSegment = ray.colNum;
        break;
      }
      ray = next;
    }

    if (hitSegment == -1)
      return false;
    if (offset == 0.0f)
      centerHitSegment = hitSegment;
    else if (hitSegment != centerHitSegment)
      return false;
  }
  return true;
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
