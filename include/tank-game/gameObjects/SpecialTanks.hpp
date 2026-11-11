#pragma once
#include "tank-game/gameObjects/EnemyTank.hpp"
namespace Tnk {
class EnemyTankSprayer : public EnemyTank {
 public:
  EnemyTankSprayer(EnemyTankCreateInfo info) : EnemyTank(info) {};
  void shoot(std::vector<Bullet>& bulletVec) override;
};

}  // namespace Tnk
