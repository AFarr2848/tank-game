#pragma once
#include "tank-game/gameObjects/EnemyTank.hpp"
namespace Tnk {
class EnemyTankSprayer : public EnemyTank {
 public:
  EnemyTankSprayer(EnemyTankCreateInfo info) : EnemyTank(info) {};
  void shoot(std::vector<Bullet>& bulletVec) override;
};
class EnemyTankChaser : public EnemyTank {
 public:
  EnemyTankChaser(EnemyTankCreateInfo info) : EnemyTank(info) {};
  void update(float deltaTime) override;
};

}  // namespace Tnk
