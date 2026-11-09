#include "tank-game/gameObjects/EnemyTank.hpp"
#include <glm/gtx/string_cast.hpp>
#include <numbers>
using namespace Tnk;
const double PI = std::numbers::pi;
void EnemyTank::updateAI() {}
void EnemyTank::update(float deltaTime) {
  sprite.bounds = bounds;
  tankAI.thinkTimer += deltaTime;
  timeSinceFired += deltaTime;
  if (tankAI.hasShot) {
    aimAtTarget(deltaTime);
    std::cout << (getAngleDifference(tankAI.shotAngle) / PI) << std::endl;
    if (canShoot()) {
      requestShoot = true;
      tankAI.hasShot = false;
    }

  } else
    moveToTarget(deltaTime);
}

bool EnemyTank::canShoot() {
  return (tankAI.hasShot && timeSinceFired >= fireCooldown &&
          currentBullets < maxBullets &&
          abs(getAngleDifference(tankAI.shotAngle)) < tankAI.shotTolerance);
}

void EnemyTank::aimAtTarget(float deltaTime) {
  float angleDiff = atan2(std::sin(tankAI.shotAngle - bounds.rotation),
                          std::cos(tankAI.shotAngle - bounds.rotation));

  float turnDirection = (angleDiff < 0.0f) ? -1.0f : 1.0f;

  turnTank(turnDirection * turnSpeed * deltaTime);
}

void EnemyTank::moveToTarget(float deltaTime) {
  glm::vec2 targetDist = bounds.center - tankAI.moveTarget;

  tankAI.desireAngle = (2 * PI) - atan2(targetDist.y, -targetDist.x);

  /*
  std::cout << "Current target: " << glm::to_string(tankAI.moveTarget)
            << std::endl;
  std::cout << "Target distance: " << glm::to_string(targetDist) << std::endl;
  std::cout << "Angle difference: " << tankAI.desireAngle - bounds.rotation
            << std::endl;
  std::cout << "Current Angle: " << bounds.rotation << std::endl;
  */

  float angleDiff = getAngleDifference(tankAI.desireAngle);

  float absDiff = abs(angleDiff);
  float turnDirection = (angleDiff < 0.0f) ? -1.0f : 1.0f;

  if (absDiff > PI / 4.0) {
    turnTank(turnDirection * turnSpeed * deltaTime);
  } else {
    moveTank(sprite.bounds.getFront() * (moveSpeed * deltaTime));
    turnTank(turnDirection * turnSpeed * deltaTime);
  }
}

float EnemyTank::getAngleDifference(float otherAngle) {
  return atan2(std::sin(otherAngle - bounds.rotation),
               std::cos(otherAngle - bounds.rotation));
}
