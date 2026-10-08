#include "tank-game/gameObjects/Tank.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <stdexcept>
#include "glm/ext/vector_float2.hpp"
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Maze.hpp"

using namespace Tnk;

void Tank::moveTank(glm::vec2 dir) {
  bounds.center += dir;
}
void Tank::turnTank(float angle) {
  bounds.rotation += angle;
}

void Tank::shoot(std::vector<Bullet>& bulletVec) {
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

glm::vec2 Tank::checkMoveCollision(float move, Maze& maze) {
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
