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
    if (canShoot()) {
      requestShoot = true;
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

  float turn =
      std::clamp(angleDiff, -turnSpeed * deltaTime, turnSpeed * deltaTime);

  turnTank(turn);
}

void EnemyTank::moveToTarget(float deltaTime) {
  tankAI.desireAngle = atan2(tankAI.moveTarget.y - bounds.center.y,
                             tankAI.moveTarget.x - bounds.center.x);

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

  float turn =
      std::clamp(angleDiff, -turnSpeed * deltaTime, turnSpeed * deltaTime);

  turnTank(turn);

  if (absDiff <= PI / 4.0) {
    moveTank(sprite.bounds.getFront() * (moveSpeed * deltaTime));
  }
}

float EnemyTank::getAngleDifference(float otherAngle) {
  return atan2(std::sin(otherAngle - bounds.rotation),
               std::cos(otherAngle - bounds.rotation));
}
