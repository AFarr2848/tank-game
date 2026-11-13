#include "tank-game/Geometry.hpp"
#include <vector>
#include "glm/geometric.hpp"
#include "tank-game/gameObjects/Maze.hpp"
using namespace Tnk;

glm::vec2 Rect::getFront() {
  return glm::normalize(glm::vec2(cos(rotation), sin(rotation)));
}

glm::mat3 Rect::getTransformation() const {
  glm::mat3 T = glm::translate(glm::mat3(1.0f), center);
  glm::mat3 R = glm::rotate(glm::mat3(1.0f), rotation);
  glm::mat3 S = glm::scale(glm::mat3(1.0f), size * 0.5f);
  return T * R * S;
}

bool Rect::intersectRay(glm::vec2 rayStart, glm::vec2 rayEnd) const {
  glm::mat3 invTransform = glm::inverse(getTransformation());

  glm::vec2 localStart = glm::vec2(invTransform * glm::vec3(rayStart, 1.0f));
  glm::vec2 localEnd = glm::vec2(invTransform * glm::vec3(rayEnd, 1.0f));

  glm::vec2 boxMin(-1.0f);
  glm::vec2 boxMax(1.0f);
  glm::vec2 dir = localEnd - localStart;

  float tmin = 0.0f;
  float tmax = 1.0f;

  if (std::abs(dir.x) < 1e-6f) {
    if (localStart.x < boxMin.x || localStart.x > boxMax.x)
      return false;
  } else {
    float ood = 1.0f / dir.x;
    float t1 = (boxMin.x - localStart.x) * ood;
    float t2 = (boxMax.x - localStart.x) * ood;
    if (t1 > t2)
      std::swap(t1, t2);
    tmin = std::max(tmin, t1);
    tmax = std::min(tmax, t2);
    if (tmin > tmax)
      return false;
  }

  if (std::abs(dir.y) < 1e-6f) {
    if (localStart.y < boxMin.y || localStart.y > boxMax.y)
      return false;
  } else {
    float ood = 1.0f / dir.y;
    float t1 = (boxMin.y - localStart.y) * ood;
    float t2 = (boxMax.y - localStart.y) * ood;
    if (t1 > t2)
      std::swap(t1, t2);
    tmin = std::max(tmin, t1);
    tmax = std::min(tmax, t2);
    if (tmin > tmax)
      return false;
  }

  return true;
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
      float difference = center.x - nearestX;
      if (difference < 0) {
        collisions.push_back(CollisionDirection::DIR_EAST);
        center.x -= (boundsRightX - nearestX);
      } else {
        collisions.push_back(CollisionDirection::DIR_WEST);
        center.x += (nearestX - boundsLeftX);
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
      float difference = center.y - nearestY;
      if (difference < 0) {
        collisions.push_back(CollisionDirection::DIR_NORTH);
        center.y -= (boundsRightY - nearestY);
      } else {
        collisions.push_back(CollisionDirection::DIR_SOUTH);
        center.y += (nearestY - boundsLeftY);
      }
    }
  }

  this->center = (center - glm::vec2(0.5)) / glm::vec2(0.5);

  return collisions;
}
