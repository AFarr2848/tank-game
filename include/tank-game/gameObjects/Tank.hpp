#pragma once
#include <iostream>
#include <numbers>
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
  float moveSpeed = 0.4;
  float turnSpeed = 2;
  float bulletSpeed = 0.6;
  float maxBullets = 2;
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

  void moveTank(glm::vec2 dir) {
    bounds.center += dir;
    light.location = bounds.center;
  }
  void turnTank(float angle) {
    bounds.rotation += angle;
    if (bounds.rotation < 0)
      bounds.rotation += 2 * std::numbers::pi;
    if (bounds.rotation > 2 * std::numbers::pi)
      bounds.rotation -= 2 * std::numbers::pi;
  }

  void shoot(std::vector<Bullet>& bulletVec) {
    int bulletsOwned = 0;
    for (Bullet& bullet : bulletVec) {
      if (bullet.owner == this)
        bulletsOwned++;
    }
    if (bulletsOwned < maxBullets) {
      requestShoot = false;
      Bullet bullet(
          Rect{

              .center = bounds.center +
                        (bounds.getFront() * bounds.size.y / glm::vec2(2)),
              .size = glm::vec2(0.03),
              .rotation = 0

          },
          *this);
      bullet.vel = bounds.getFront() * glm::vec2(bulletSpeed);
      bulletVec.push_back(bullet);
    }
  }

  virtual ~Tank() {}
  virtual void update(float deltaTime) = 0;

  Rect bounds;
  Sprite sprite;
  PointLight light;

  float bulletSpeed;
  int maxBullets;
  float moveSpeed;
  float turnSpeed;
  bool requestShoot = false;
};

}  // namespace Tnk
