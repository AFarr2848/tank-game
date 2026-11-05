#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {

struct PlayerCreateInfo : public TankCreateInfo {};

class PlayerTank : public Tank {
 public:
  PlayerTank(PlayerCreateInfo info) : Tank(info) {}
  PlayerTank() : Tank() {}

  double mathTimer = 0;

  void update(float deltaTime) override;

  void checkTurnCollision(float turnTry);
  glm::vec2 checkMoveCollision(float move, Maze& maze);
};
}  // namespace Tnk
