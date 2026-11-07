#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {
struct TankAI {
  glm::vec2 moveTarget = glm::vec2(-1);
  float thinkTimer = 0;
  float shotAngle = 0;
  bool hasShot = false;
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
  float getAngleDifference();

  void aimAtTarget(float deltaTime);
  void moveToTarget(float deltaTime);
};

}  // namespace Tnk
