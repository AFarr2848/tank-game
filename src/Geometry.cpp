#include "tank-game/Geometry.hpp"
#include <vector>
#include "tank-game/gameObjects/Maze.hpp"
using namespace Tnk;

glm::mat3 Rect::getTransformation() const {
  glm::mat3 T = glm::translate(glm::mat3(1.0f), center);
  glm::mat3 R = glm::rotate(glm::mat3(1.0f), rotation);
  glm::mat3 S = glm::scale(glm::mat3(1.0f), size * 0.5f);
  return T * R * S;
}

std::vector<CollisionDirection> Rect::collideMaze(Maze& maze) {
  std::vector<CollisionDirection> collisions;
  glm::vec2 center = normalizedCenter();

  float halfWidthX =
      (size.x / 2 * abs(sin(rotation)) + size.y / 2 * abs(cos(rotation))) *
      0.5f;
  float halfWidthY =
      (size.x / 2 * abs(cos(rotation)) + size.y / 2 * abs(sin(rotation))) *
      0.5f;

  int nearestWallCoordX = std::round(center.x * maze.mazeSize);
  float nearestX = float(nearestWallCoordX) / maze.mazeSize;

  float boundsLeftX = center.x - halfWidthX;
  float boundsRightX = center.x + halfWidthX;

  if (boundsLeftX < nearestX && boundsRightX > nearestX) {
    int cellRow =
        glm::clamp(int(center.y * maze.mazeSize), 0, maze.mazeSize - 1);

    bool isWallPresent = false;
    if (nearestWallCoordX > 0 && nearestWallCoordX < maze.mazeSize) {
      isWallPresent = maze.cells.at(cellRow).at(nearestWallCoordX).wallWest ||
                      maze.cells.at(cellRow).at(nearestWallCoordX - 1).wallEast;
    } else if (nearestWallCoordX == 0 || nearestWallCoordX == maze.mazeSize) {
      isWallPresent = true;
    }

    if (isWallPresent) {
      if (center.x < nearestX) {
        collisions.push_back(CollisionDirection::DIR_EAST);

      } else if (center.x > nearestX) {
        collisions.push_back(CollisionDirection::DIR_WEST);
      }
    }
  }

  int nearestWallCoordY = std::round(center.y * maze.mazeSize);
  float nearestY = float(nearestWallCoordY) / maze.mazeSize;

  float boundsLeftY = center.y - halfWidthY;
  float boundsRightY = center.y + halfWidthY;

  if (boundsLeftY < nearestY && boundsRightY > nearestY) {
    int cellCol =
        glm::clamp(int(center.x * maze.mazeSize), 0, maze.mazeSize - 1);

    bool isWallPresent = false;
    if (nearestWallCoordY > 0 && nearestWallCoordY < maze.mazeSize) {
      isWallPresent =
          maze.cells.at(nearestWallCoordY).at(cellCol).wallNorth ||
          maze.cells.at(nearestWallCoordY - 1).at(cellCol).wallSouth;
    } else if (nearestWallCoordY == 0 || nearestWallCoordY == maze.mazeSize) {
      isWallPresent = true;
    }

    if (isWallPresent) {
      if (center.y < nearestY) {
        collisions.push_back(CollisionDirection::DIR_NORTH);
      } else if (center.y > nearestY) {
        collisions.push_back(CollisionDirection::DIR_SOUTH);
      }
    }
  }
  return collisions;
};
