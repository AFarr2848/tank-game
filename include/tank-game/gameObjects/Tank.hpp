#pragma once
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Bullet.hpp"
#include "tank-game/gameObjects/Sprite.hpp"

namespace Tnk {
class Maze;
class Tank {
 public:
  Tank(Rect bounds) : bounds(bounds), sprite(this->bounds) {}
  Tank() : bounds(), sprite(this->bounds) {
    light = PointLight{.location = bounds.center,
                       .radius = 0.03,
                       .color = glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(10)};
  }

  Tank(const Tank& other) : bounds(other.bounds), sprite(this->bounds) {
    sprite.texture = other.sprite.texture;
    light = PointLight{.location = bounds.center,
                       .radius = 0.03,
                       .color = glm::vec3(3.0f, 3.0f, 3.0f)};
  }

  Tank& operator=(const Tank& other) {
    if (this != &other) {
      bounds = other.bounds;
      sprite = other.sprite;
      sprite.bounds = this->bounds;
    }
    return *this;
  }

  bool operator==(const Tank& other) const { return this == &other; }

  Rect bounds;
  Sprite sprite;
  PointLight light;

  void moveTank(glm::vec2 dir);
  void turnTank(float angle);
  void shoot(std::vector<Bullet>& bulletVec);

  void checkTurnCollision(float turnTry);
  glm::vec2 checkMoveCollision(float move, Maze& maze);

 private:
  std::vector<Bullet> bullets;
};

}  // namespace Tnk
