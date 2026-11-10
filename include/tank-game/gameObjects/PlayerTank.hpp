#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {

struct PlayerCreateInfo : public TankCreateInfo {};

class InputHelper;

class PlayerTank : public Tank {
 public:
  PlayerTank(PlayerCreateInfo info) : Tank(info) {}
  PlayerTank() : Tank() {}

  double mathTimer = 0;

  void updatePlayer(float deltaTime,
                    InputHelper& inputHelper,
                    Maze& maze,
                    std::vector<Bullet>& bulletVec);

 private:
  void checkTurnCollision(float turnTry);
  glm::vec2 checkMoveCollision(float move, Maze& maze);
  void updateInputs(float deltaTime,
                    InputHelper& inputHelper,
                    Maze& maze,
                    std::vector<Bullet>& bulletVec);

 protected:
  void update(float deltaTime) override;
};
}  // namespace Tnk
