#pragma once
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Sprite.hpp"

namespace Tnk {
class Maze;
class Tank {
 public:
  Tank(Rect bounds) : bounds(bounds), sprite(&this->bounds) {}
  Tank() : bounds(), sprite(&this->bounds) {}

  Tank(const Tank& other) : bounds(other.bounds), sprite(&this->bounds) {
    sprite.texture = other.sprite.texture;
  }

  Tank& operator=(const Tank& other) {
    if (this != &other) {
      bounds = other.bounds;
      sprite = other.sprite;
      sprite.bounds = &this->bounds;
    }
    return *this;
  }

  Rect bounds;
  Sprite sprite;

  void moveTank(glm::vec2 dir);
  void turnTank(float angle);

  void checkTurnCollision(float turnTry);
  glm::vec2 checkMoveCollision(float move, Maze& maze);

 private:
};

}  // namespace Tnk
