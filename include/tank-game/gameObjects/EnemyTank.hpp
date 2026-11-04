#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {
struct TankAI {
  std::vector<glm::ivec2> path;
  float thinkTimer = 0;
  float desireAngle;
  int maxRicochets;
};
struct EnemyTankCreateInfo : public TankCreateInfo {
  int tankValue = 1;
  int maxRicochets = 1;
};

class EnemyTank : public Tank {
 public:
  EnemyTank(EnemyTankCreateInfo info) : Tank(info) {
    tankAI.maxRicochets = info.maxRicochets;
    tankValue = info.tankValue;
  };

  int tankValue;
  TankAI tankAI;

  void updateAI();
  void updateMovement();
  void update(float deltaTime) override;
};

}  // namespace Tnk
