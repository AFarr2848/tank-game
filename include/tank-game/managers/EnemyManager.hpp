#pragma once
#include <unordered_map>
#include "tank-game/gameObjects/EnemyTank.hpp"
namespace Tnk {
class PlayerTank;
class EnemyManager {
 public:
  std::vector<std::unique_ptr<EnemyTank>> enemyTanks;
  std::unordered_map<std::string, EnemyTankCreateInfo> enemyTypes;
  int maxTankValue = 1;

  void addTank(std::string type);
  std::unique_ptr<EnemyTank>* getEnemyByID(uint32_t id);
  void updateEnemies(PlayerTank& playerTank,
                     Maze& maze,
                     std::vector<Bullet>& bulletVec,
                     float deltaTime);
  void updateEnemyShots(PlayerTank& playerTank, Maze& maze);
  int getTotalTankValues();
  void killTank(uint32_t id);
  void initEnemyTypes();

 private:
  struct ShotRay {
    glm::vec2 pos;
    float angle;
    int colNum;
  };

  std::string nextTankSpawn;

  std::string getRandomTankName(int maxValue);
  void updateEnemyAI(EnemyTank& tank, PlayerTank& playerTank, Maze& maze);
  void checkAvailableShots();
  std::vector<glm::vec2> findEnemyPath(glm::ivec2 start,
                                       glm::ivec2 end,
                                       Maze& maze);

  ShotRay calculateRayBounce(ShotRay ray, Maze& maze);

  void trySpawnTank(PlayerTank& playerTank, Maze& maze);
  glm::vec2 getSpawnLocation(PlayerTank& playerTank, Maze& maze);

  bool verifyShot(const EnemyTank& shooter,
                  float angle,
                  const PlayerTank& player,
                  Maze& maze);
};

}  // namespace Tnk
