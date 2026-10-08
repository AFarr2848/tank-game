#pragma once
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Bullet.hpp"
#include "tank-game/gameObjects/Sprite.hpp"

namespace Tnk {
class Maze;
class Tank {
 public:
  Tank(Rect bounds) : bounds(bounds), sprite(this->bounds) {}
  Tank() : bounds(), sprite(this->bounds) {}

  Tank(const Tank& other) : bounds(other.bounds), sprite(this->bounds) {
    sprite.texture = other.sprite.texture;
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

  void moveTank(glm::vec2 dir);
  void turnTank(float angle);
  void shoot(std::vector<Bullet>& bulletVec);

  void checkTurnCollision(float turnTry);
  glm::vec2 checkMoveCollision(float move, Maze& maze);

 private:
  std::vector<Bullet> bullets;
};

}  // namespace Tnk
