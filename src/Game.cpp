#include "tank-game/Game.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <memory>
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
  enemyManager.initEnemyTypes();
  PlayerCreateInfo playerInfo;
  playerInfo.bounds = {
      .center = {-0.9, -0.9}, .size = {0.11, 0.075}, .rotation = 0};
  playerInfo.texture = "playerTank";
  playerInfo.lightColor = glm::vec3(10.0f);
  playerInfo.moveSpeed = 0.4;
  playerInfo.turnSpeed = 2;

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

  for (const auto& enemy : enemyManager.enemyTanks) {
    if (currentLightIndex < MAX_LIGHTS) {
      lights[currentLightIndex++] = enemy->light;
    }
  }

  numLights = currentLightIndex;
  return lights;
}

void Game::getMazeLines() {
  mazeLines = maze.makeLines();
}

void Game::updateGame() {
  if (!gameOver) {
    if (playerTank.mathTimer > AI_THINK_INTERVAL) {
      playerTank.mathTimer = 0;
      enemyManager.updateEnemyShots(playerTank, maze);
    }

    enemyManager.updateEnemies(playerTank, maze, bulletVec,
                               timingData.deltaTime);

    gameOver = enemyManager.checkPlayerCollisions(playerTank);
    collideBullets();
    playerTank.updatePlayer(timingData.deltaTime, inputHelper, maze, bulletVec);
  }
}

void Game::collideBullets() {
  for (int i = 0; i < bulletVec.size(); i++) {
    Bullet& b = bulletVec.at(i);
    b.moveBullet(timingData.deltaTime, maze);
    if (b.bounces > b.maxBounces) {
      Tank* t;
      if (b.ownerID == playerTank.id)
        t = &playerTank;
      else if (enemyManager.getEnemyByID(b.ownerID) != nullptr)
        t = enemyManager.getEnemyByID(b.ownerID)->get();
      if (t != nullptr)
        t->currentBullets--;
    }
  }
  std::erase_if(bulletVec, [](auto& b) { return b.bounces > b.maxBounces; });

  for (auto& b : bulletVec) {
    if (b.ownerID != playerTank.id &&
        playerTank.bounds.contains(b.bounds.center))
      gameOver = true;

    std::vector<EnemyTank*> toRemove;
    for (auto& t : enemyManager.enemyTanks) {
      if (b.ownerID == playerTank.id && t->bounds.contains(b.bounds.center))
        toRemove.push_back(&(*t));
    }
    for (auto t : toRemove)
      enemyManager.killTank(t->id);
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
  for (auto& t : enemyManager.enemyTanks) {
    t->sprite.bounds = t->bounds;
    sprites.push_back(t->sprite);
  }

  return sprites;
}
