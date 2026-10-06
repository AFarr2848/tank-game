#include "tank-game/gameObjects/Tank.hpp"
#include <glm/gtx/string_cast.hpp>
#include <stdexcept>
#include "glm/ext/vector_float2.hpp"
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

  glm::vec2 center = bounds.normalizedCenter();

  float halfWidthX = (bounds.size.x / 2 * abs(sin(bounds.rotation)) +
                      bounds.size.y / 2 * abs(cos(bounds.rotation))) *
                     0.5f;
  float halfWidthY = (bounds.size.x / 2 * abs(cos(bounds.rotation)) +
                      bounds.size.y / 2 * abs(sin(bounds.rotation))) *
                     0.5f;

  int nearestWallCoordX = std::round(center.x * maze.mazeSize);
  float nearestX = float(nearestWallCoordX) / maze.mazeSize;

  float boundsLeftX = center.x - halfWidthX;
  float boundsRightX = center.x + halfWidthX;

  if (boundsLeftX < nearestX && boundsRightX > nearestX) {
    int cellRow =
        std::clamp(int(center.y * maze.mazeSize), 0, maze.mazeSize - 1);

    bool isWallPresent = false;
    if (nearestWallCoordX > 0 && nearestWallCoordX < maze.mazeSize) {
      isWallPresent = maze.cells.at(cellRow).at(nearestWallCoordX).wallWest ||
                      maze.cells.at(cellRow).at(nearestWallCoordX - 1).wallEast;
    } else if (nearestWallCoordX == 0 || nearestWallCoordX == maze.mazeSize) {
      isWallPresent = true;
    }

    if (isWallPresent) {
      if (center.x < nearestX && moveVec.x > 0.0f) {
        moveVec.x = 0.0f;
      } else if (center.x > nearestX && moveVec.x < 0.0f) {
        moveVec.x = 0.0f;
      }
    }
  }

  int nearestWallCoordY = std::round(center.y * maze.mazeSize);
  float nearestY = float(nearestWallCoordY) / maze.mazeSize;

  float boundsLeftY = center.y - halfWidthY;
  float boundsRightY = center.y + halfWidthY;

  if (boundsLeftY < nearestY && boundsRightY > nearestY) {
    int cellCol =
        std::clamp(int(center.x * maze.mazeSize), 0, maze.mazeSize - 1);

    bool isWallPresent = false;
    if (nearestWallCoordY > 0 && nearestWallCoordY < maze.mazeSize) {
      isWallPresent =
          maze.cells.at(nearestWallCoordY).at(cellCol).wallNorth ||
          maze.cells.at(nearestWallCoordY - 1).at(cellCol).wallSouth;
    } else if (nearestWallCoordY == 0 || nearestWallCoordY == maze.mazeSize) {
      isWallPresent = true;
    }

    if (isWallPresent) {
      if (center.y < nearestY && moveVec.y > 0.0f) {
        moveVec.y = 0.0f;
      } else if (center.y > nearestY && moveVec.y < 0.0f) {
        moveVec.y = 0.0f;
      }
    }
  }

  return moveVec;
}
