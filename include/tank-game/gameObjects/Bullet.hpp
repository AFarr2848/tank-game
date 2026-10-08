#pragma once
#include "glm/ext/vector_float2.hpp"
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Sprite.hpp"
namespace Tnk {
class Tank;

class Bullet {
 public:
  Bullet(Rect bounds, Tank& owner)
      : bounds(bounds), sprite(this->bounds), owner(&owner) {
    sprite.texture = "bullet";
  }
  Bullet(Tank& owner) : bounds(), sprite(this->bounds), owner(&owner) {
    sprite.texture = "bullet";
  }

  Bullet& operator=(const Bullet& other) {
    if (this != &other) {
      vel = other.vel;
      bounds = other.bounds;
      sprite = other.sprite;
      owner = other.owner;
      bounces = other.bounces;
      maxBounces = other.maxBounces;
    }
    return *this;
  }

  glm::vec2 vel;
  Rect bounds;
  Sprite sprite;
  Tank* owner;
  int bounces = 0;
  int maxBounces = 2;

  void moveBullet(float deltaTime, Maze& maze);
  void collideBullet();
};

}  // namespace Tnk
