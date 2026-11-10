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
  float fireCooldown = 0.1;
  int maxRicochets = 2;
};

class Tank {
 public:
  Tank(TankCreateInfo info)
      : id(nextID++), bounds(info.bounds), sprite(this->bounds) {
    light = PointLight{.location = this->bounds.center,
                       .radius = 0.02,
                       .color = info.lightColor};
    moveSpeed = info.moveSpeed;
    turnSpeed = info.turnSpeed;
    sprite.texture = info.texture;
    bulletSpeed = info.bulletSpeed;
    maxBullets = info.maxBullets;
    fireCooldown = info.fireCooldown;
    maxRicochets = info.maxRicochets;
  }

  Tank() : id(nextID++), bounds(), sprite(this->bounds) {
    TankCreateInfo info = TankCreateInfo();
    light = PointLight{.location = this->bounds.center,
                       .radius = 0.02,
                       .color = info.lightColor};
    moveSpeed = info.moveSpeed;
    turnSpeed = info.turnSpeed;
    sprite.texture = info.texture;
    bulletSpeed = info.bulletSpeed;
    maxBullets = info.maxBullets;
    fireCooldown = info.fireCooldown;
    maxRicochets = info.maxRicochets;
  }

  Tank(const Tank& other)
      : id(other.id), bounds(other.bounds), sprite(this->bounds) {
    light = PointLight{.location = bounds.center,
                       .radius = 0.02,
                       .color = glm::vec3(3.0f, 3.0f, 3.0f)};
    moveSpeed = other.moveSpeed;
    turnSpeed = other.turnSpeed;
    sprite.texture = other.sprite.texture;
    bulletSpeed = other.bulletSpeed;
    maxBullets = other.maxBullets;
    fireCooldown = other.fireCooldown;
    timeSinceFired = other.timeSinceFired;
    maxRicochets = other.maxRicochets;
  }

  Tank& operator=(const Tank& other) {
    if (this != &other) {
      bounds = other.bounds;
      sprite = other.sprite;
      sprite.bounds = this->bounds;
      id = other.id;
      moveSpeed = other.moveSpeed;
      turnSpeed = other.turnSpeed;
      bulletSpeed = other.bulletSpeed;
      maxBullets = other.maxBullets;
      fireCooldown = other.fireCooldown;
      timeSinceFired = other.timeSinceFired;
      maxRicochets = other.maxRicochets;
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
    requestShoot = false;
    timeSinceFired = 0;
    currentBullets++;
    Bullet bullet(
        Rect{

            .center = bounds.center +
                      (bounds.getFront() * bounds.size.y / glm::vec2(2)),
            .size = glm::vec2(0.03),
            .rotation = 0

        },
        this->id, this->maxRicochets);
    bullet.vel = bounds.getFront() * bulletSpeed;
    bulletVec.push_back(bullet);
  }

  virtual ~Tank() {}
  virtual void update(float deltaTime) = 0;

  Rect bounds;
  Sprite sprite;
  PointLight light;

  uint32_t id;

  float bulletSpeed;
  int maxBullets;
  int currentBullets = 0;
  float moveSpeed;
  float turnSpeed;
  bool requestShoot = false;
  float fireCooldown;
  float timeSinceFired = 0;
  int maxRicochets;

 private:
  static inline uint32_t nextID = 1;
};

}  // namespace Tnk
