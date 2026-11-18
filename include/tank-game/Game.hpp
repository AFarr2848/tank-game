#pragma once
#include "tank-game/gameObjects/Maze.hpp"
#include "tank-game/gameObjects/PlayerTank.hpp"
#include "tank-game/managers/EnemyManager.hpp"

namespace Tnk {
const float AI_THINK_INTERVAL = 0.3;
class InputHelper;
class Gui;
struct TimingData;
class Game {
 public:
  Game(InputHelper& inputHelper, TimingData& timingData, Gui& gui)
      : inputHelper(inputHelper), timingData(timingData), gui(gui) {};

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

  bool gameOver = false;
  bool gamePaused = false;

 private:
  EnemyManager enemyManager;

  InputHelper& inputHelper;
  TimingData& timingData;
  Gui& gui;
  int maxTankValue = 1;

  void collideBullets();
  void processInput();

  std::vector<Bullet> bulletVec;
};

}  // namespace Tnk
