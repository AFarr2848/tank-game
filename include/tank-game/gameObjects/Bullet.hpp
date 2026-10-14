#pragma once
#include "glm/ext/vector_float2.hpp"
#include "tank-game/Geometry.hpp"
#include "tank-game/Lighting.hpp"
#include "tank-game/gameObjects/Sprite.hpp"
namespace Tnk {
class Tank;

class Bullet {
 public:
  Bullet(Rect bounds, Tank& owner)
      : bounds(bounds), sprite(this->bounds), owner(&owner) {
    sprite.texture = "bullet";
    light = PointLight{.location = bounds.center,
                       .radius = 0.01,
                       .color = glm::vec3(0.5f, 0.0f, 0.2f) * glm::vec3(25.0)};
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
  PointLight light;
  int bounces = 0;
  int maxBounces = 2;

  void moveBullet(float deltaTime, Maze& maze);
  void collideBullet();
};

}  // namespace Tnk
