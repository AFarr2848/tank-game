#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {

struct PlayerCreateInfo : public TankCreateInfo {};

class PlayerTank : public Tank {
 public:
  PlayerTank(PlayerCreateInfo info) : Tank(info) {}
  PlayerTank() : Tank() {}
  void moveTank(glm::vec2 dir);
  void turnTank(float angle);
  void shoot(std::vector<Bullet>& bulletVec);
  void update(float deltaTime) override;

  void checkTurnCollision(float turnTry);
  glm::vec2 checkMoveCollision(float move, Maze& maze);
};
}  // namespace Tnk
