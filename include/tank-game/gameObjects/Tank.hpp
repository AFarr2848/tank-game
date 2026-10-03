#pragma once
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Sprite.hpp"

namespace Tnk {
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

  void moveTank(float speed);
  void turnTank(float angle);

 private:
};

}  // namespace Tnk
