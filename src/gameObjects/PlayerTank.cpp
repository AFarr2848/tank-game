
#include "tank-game/gameObjects/PlayerTank.hpp"
using namespace Tnk;

void PlayerTank::update(float deltaTime) {
  sprite.bounds = bounds;
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
