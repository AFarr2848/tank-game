#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {
struct TankAI {
  glm::vec2 moveTarget = glm::vec2(-1);
  float thinkTimer = 0;
  float shotAngle = 0;
  bool hasShot = false;
  float desireAngle;
  float shotTolerance;
};
struct EnemyTankCreateInfo : public TankCreateInfo {
  int tankValue = 1;
  int maxRicochets = 1;
  float shotTolerance = std::numbers::pi / 32;
};

class EnemyTank : public Tank {
 public:
  EnemyTank(EnemyTankCreateInfo info) : Tank(info) {
    maxRicochets = info.maxRicochets;
    tankValue = info.tankValue;
    tankAI.shotTolerance = info.shotTolerance;
  };

  int tankValue;
  TankAI tankAI;

  void updateAI();
  bool canShoot();
  void updateMovement();
  void update(float deltaTime) override;
  float getAngleDifference(float otherAngle);
  void aimAtTarget(float deltaTime);
  void moveToTarget(float deltaTime);
};

}  // namespace Tnk
