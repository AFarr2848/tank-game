#pragma once
#include "tank-game/Geometry.hpp"
#include "tank-game/Lighting.hpp"
#include "tank-game/gameObjects/Bullet.hpp"
#include "tank-game/gameObjects/Sprite.hpp"

namespace Tnk {

class Maze;

struct TankCreateInfo {
  Rect bounds = Rect();
  std::string texture = "playerTank";
  glm::vec3 lightColor = {10.0f, 10.0f, 10.0f};
  float moveSpeed = 0.3;
  float turnSpeed = 0.05;
};

class Tank {
 public:
  Tank(TankCreateInfo info) : bounds(info.bounds), sprite(this->bounds) {
    light = PointLight{.location = this->bounds.center,
                       .radius = 0.02,
                       .color = info.lightColor};
    moveSpeed = info.moveSpeed;
    turnSpeed = info.turnSpeed;
    sprite.texture = info.texture;
  }

  Tank() : bounds(), sprite(this->bounds) {
    TankCreateInfo info = TankCreateInfo();
    light = PointLight{.location = this->bounds.center,
                       .radius = 0.02,
                       .color = info.lightColor};
    moveSpeed = info.moveSpeed;
    turnSpeed = info.turnSpeed;
    sprite.texture = info.texture;
  }

  Tank(const Tank& other) : bounds(other.bounds), sprite(this->bounds) {
    sprite.texture = other.sprite.texture;
    light = PointLight{.location = bounds.center,
                       .radius = 0.02,
                       .color = glm::vec3(3.0f, 3.0f, 3.0f)};
    moveSpeed = other.moveSpeed;
    turnSpeed = other.turnSpeed;
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

  virtual ~Tank() {}

  Rect bounds;
  Sprite sprite;
  PointLight light;

  virtual void update(float deltaTime) = 0;

  float moveSpeed;
  float turnSpeed;
};

}  // namespace Tnk
