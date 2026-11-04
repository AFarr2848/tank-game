
#include "tank-game/gameObjects/PlayerTank.hpp"
using namespace Tnk;

void PlayerTank::moveTank(glm::vec2 dir) {
  bounds.center += dir;
  light.location = bounds.center;
}
void PlayerTank::turnTank(float angle) {
  bounds.rotation += angle;
}

void PlayerTank::update(float deltaTime) {
  sprite.bounds = bounds;
}

void PlayerTank::shoot(std::vector<Bullet>& bulletVec) {
  int bulletsOwned = 0;
  for (Bullet& bullet : bulletVec) {
    if (bullet.owner == this)
      bulletsOwned++;
  }
  if (bulletsOwned < 2) {
    Bullet bullet(
        Rect{

            .center = bounds.center +
                      (bounds.getFront() * bounds.size.y / glm::vec2(2)),
            .size = glm::vec2(0.03),
            .rotation = 0

        },
        *this);
    bullet.vel = bounds.getFront() * glm::vec2(0.6);
    bulletVec.push_back(bullet);
  }
}

glm::vec2 PlayerTank::checkMoveCollision(float move, Maze& maze) {
  glm::vec2 moveVec = glm::vec2(std::cos(bounds.rotation) * move,
                                std::sin(bounds.rotation) * move);

  Rect nextBounds = Rect(bounds);
  nextBounds.center += moveVec;

  for (auto colDir : bounds.collideMaze(maze)) {
    if (colDir == CollisionDirection::DIR_NORTH && moveVec.y > 0)
      moveVec.y = 0;
    if (colDir == CollisionDirection::DIR_SOUTH && moveVec.y < 0)
      moveVec.y = 0;
    if (colDir == CollisionDirection::DIR_EAST && moveVec.x > 0)
      moveVec.x = 0;
    if (colDir == CollisionDirection::DIR_WEST && moveVec.x < 0)
      moveVec.x = 0;
  }
  return moveVec;
}
