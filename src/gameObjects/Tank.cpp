#include "tank-game/gameObjects/Tank.hpp"
#include <glm/gtx/string_cast.hpp>

using namespace Tnk;
void Tank::moveTank(float speed) {
  bounds.center +=
      glm::vec2(cos(bounds.rotation) * speed, sin(bounds.rotation) * speed);
  std::cout << "New center: " + glm::to_string(bounds.center) << std::endl;
  std::cout << "Sprite center: " + glm::to_string(sprite.bounds->center)
            << std::endl;
}
void Tank::turnTank(float angle) {
  bounds.rotation += angle;
}
