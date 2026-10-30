#pragma once
#include "tank-game/gameObjects/Tank.hpp"
namespace Tnk {
class TankAI {
  std::vector<glm::ivec2> path;
  float thinkTimer = 0;
  float aimAngle = 0;
  int maxRicochets = 0;
};

class EnemyTank : public Tank {
 public:
  EnemyTank(Rect bounds, std::string sprite, glm::vec3 lightColor)
      : Tank(bounds) {
    this->sprite.texture = sprite;
    this->light.color = lightColor;
  };

  int tankValue = 1;
  TankAI tankAI;

  void updateAI();
};

}  // namespace Tnk
