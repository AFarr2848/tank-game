#include "tank-game/gameObjects/Tank.hpp"
#include <glm/gtx/string_cast.hpp>
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
