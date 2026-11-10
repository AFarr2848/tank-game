#pragma once
#include "tank-game/gameObjects/Maze.hpp"
#include "tank-game/gameObjects/PlayerTank.hpp"
#include "tank-game/managers/EnemyManager.hpp"

namespace Tnk {
const float AI_THINK_INTERVAL = 0.3;
class InputHelper;
struct TimingData;
class Game {
 public:
  Game(InputHelper& inputHelper, TimingData& timingData)
      : inputHelper(inputHelper), timingData(timingData) {};

  void startGame();
  void getMazeLines();
  std::vector<glm::vec2> mazeLines;
  PlayerTank playerTank;
  void updateGame();
  int mazeSize;
  int numLights;
  Maze maze;

  std::vector<Sprite> getSprites();
  std::array<PointLight, MAX_LIGHTS> getLights();

 private:
  EnemyManager enemyManager;

  InputHelper& inputHelper;
  TimingData& timingData;
  int maxTankValue = 1;

  void collideBullets();

  std::vector<Bullet> bulletVec;
};

}  // namespace Tnk
