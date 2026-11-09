#pragma once
#include "tank-game/gameObjects/EnemyTank.hpp"
#include "tank-game/gameObjects/Maze.hpp"
#include "tank-game/gameObjects/PlayerTank.hpp"

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
  std::vector<EnemyTank> enemyTanks;
  void updateGame();
  int mazeSize;
  int numLights;
  Maze maze;

  std::vector<Sprite> getSprites();
  std::array<PointLight, MAX_LIGHTS> getLights();

 private:
  struct ShotRay {
    glm::vec2 pos;
    float angle;
    int colNum;
  };

  InputHelper& inputHelper;
  TimingData& timingData;
  int maxTankValue = 1;
  void spawnTank(EnemyTank tank);
  void trySpawnTank();
  Tank* getTankByID(uint32_t id);

  void collideBullets();
  void updateTankAI(EnemyTank& tank);

  int getTotalTankValues();
  glm::vec2 getSpawnLocation();
  glm::ivec2 getTankMazeCoords(Tank& tank);
  glm::vec2 mazeToNdcCoords(glm::ivec2 mazeCoords);
  std::vector<glm::vec2> findMazePath(glm::ivec2 start, glm::ivec2 end);
  Game::ShotRay calculateRayBounce(ShotRay ray);
  void checkAvailableShots();

  void getInputs();

  std::vector<Bullet> bulletVec;
};

}  // namespace Tnk
