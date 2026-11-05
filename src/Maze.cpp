#include "tank-game/gameObjects/Maze.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <random>
#include <vector>
using namespace Tnk;

std::vector<glm::vec2> Maze::makeLines() {
  std::vector<glm::vec2> lines;
  lines.reserve(mazeSize * mazeSize * 4);

  auto toNDC = [this](float x, float y) -> glm::vec2 {
    float scale = 2.0f / mazeSize;
    glm::vec2 newPoint = {(x * scale) - 1.0f, (y * scale) - 1.0f};
    if (newPoint.x == -1)
      newPoint.x = -0.999;
    if (newPoint.y == -1)
      newPoint.y = -0.999;
    return newPoint;
  };
  for (size_t row = 0; row < cells.size(); ++row) {
    for (size_t col = 0; col < cells[row].size(); ++col) {
      const auto& cell = cells[row][col];

      if (cell.wallEast) {
        lines.push_back(toNDC(col + 1, row));
        lines.push_back(toNDC(col + 1, row + 1));
      }

      if (cell.wallSouth) {
        lines.push_back(toNDC(col, row + 1));
        lines.push_back(toNDC(col + 1, row + 1));
      }

      if (col == 0 && cell.wallWest) {
        lines.push_back(toNDC(col, row));
        lines.push_back(toNDC(col, row + 1));
      }

      if (row == 0 && cell.wallNorth) {
        lines.push_back(toNDC(col, row));
        lines.push_back(toNDC(col + 1, row));
      }
    }
  }

  return lines;
}

bool Maze::canCarveInDirection(glm::vec2 cellCoords, glm::vec2 dir) {
  glm::vec2 mazeCoords = cellCoords + dir;

  if (mazeCoords.x < 0 || mazeCoords.x >= mazeSize || mazeCoords.y < 0 ||
      mazeCoords.y >= mazeSize) {
    return false;
  }

  int x = static_cast<int>(mazeCoords.x);
  int y = static_cast<int>(mazeCoords.y);

  return !cells.at(y).at(x).visited;
}

void Maze::carveFrom(glm::vec2 cellCoords) {
  using namespace std;
  vector<glm::vec2> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  shuffle(directions.begin(), directions.end(), gen);
  for (glm::vec2 dir : directions) {
    if (canCarveInDirection(cellCoords, dir)) {
      glm::vec2 newCell = cellCoords + dir;
      glm::vec2 reverseCoords = dir * glm::vec2{-1, -1};
      changeWalls(cellCoords, dir);
      changeWalls(newCell, reverseCoords);
      carveFrom(newCell);
    }
  }
}

void Maze::changeWalls(glm::vec2 cellCoords, glm::vec2 carveDir) {
  MazeCell& cell = cells.at(cellCoords.y).at(cellCoords.x);
  if (carveDir.x == 1)
    cell.wallEast = false;
  if (carveDir.x == -1)
    cell.wallWest = false;
  if (carveDir.y == 1)
    cell.wallSouth = false;
  if (carveDir.y == -1)
    cell.wallNorth = false;
  cell.visited = true;
}

std::vector<glm::ivec2> Maze::getConnectedCells(glm::ivec2 cellCoords) {
  assert(cellCoords.x < mazeSize && cellCoords.y < mazeSize);
  MazeCell& cell = cells.at(cellCoords.x).at(cellCoords.y);
  std::vector<glm::ivec2> dirs;
  if (!cell.wallNorth && cellCoords.x > 0)
    dirs.push_back({cellCoords.x - 1, cellCoords.y});
  if (!cell.wallSouth && cellCoords.x < mazeSize - 1)
    dirs.push_back({cellCoords.x + 1, cellCoords.y});
  if (!cell.wallWest && cellCoords.y > 0)
    dirs.push_back({cellCoords.x, cellCoords.y - 1});
  if (!cell.wallEast && cellCoords.y < mazeSize - 1)
    dirs.push_back({cellCoords.x, cellCoords.y + 1});
  return dirs;
}

void Maze::makeMaze(int mazeSize) {
  using namespace std;
  this->mazeSize = mazeSize;
  this->cells.resize(mazeSize);
  for (auto& vec : cells)
    vec.resize(mazeSize);

  gen = std::mt19937(rd());
  uniform_int_distribution<> distrib(0, mazeSize - 1);
  glm::vec2 startingCell = {distrib(gen), distrib(gen)};
  carveFrom(startingCell);
}

// i couldnt be assed to write this myself tbh
Maze::RayBounceResult Maze::calculateRayBounce(glm::vec2 startPos,
                                               float angle) {
  // 1. Calculate direction vector from angle
  glm::vec2 dir = {std::cos(angle), std::sin(angle)};

  // 2. Determine starting grid cell coordinate
  int mapX = static_cast<int>(std::floor(startPos.x));
  int mapY = static_cast<int>(std::floor(startPos.y));

  // 3. DDA Step setup
  glm::vec2 deltaDist = {(dir.x == 0) ? 1e30f : std::abs(1.0f / dir.x),
                         (dir.y == 0) ? 1e30f : std::abs(1.0f / dir.y)};

  int stepX = (dir.x < 0) ? -1 : 1;
  int stepY = (dir.y < 0) ? -1 : 1;

  glm::vec2 sideDist;
  if (dir.x < 0) {
    sideDist.x = (startPos.x - mapX) * deltaDist.x;
  } else {
    sideDist.x = (mapX + 1.0f - startPos.x) * deltaDist.x;
  }

  if (dir.y < 0) {
    sideDist.y = (startPos.y - mapY) * deltaDist.y;
  } else {
    sideDist.y = (mapY + 1.0f - startPos.y) * deltaDist.y;
  }

  bool hitWall = false;
  int hitSide =
      0;  // 0 = X-axis wall (East/West), 1 = Y-axis wall (North/South)

  // 4. Step through grid cells
  while (!hitWall) {
    // Jump to next grid cell in ray path
    if (sideDist.x < sideDist.y) {
      sideDist.x += deltaDist.x;
      mapX += stepX;
      hitSide = 0;
    } else {
      sideDist.y += deltaDist.y;
      mapY += stepY;
      hitSide = 1;
    }

    // 5. Check actual maze walls based on which boundary we crossed
    const auto& cell = cells[mapY][mapX];
    if (hitSide == 0) {
      // Crossed a vertical line (West or East wall)
      if (stepX > 0 && cell.wallEast)
        hitWall = true;
      else if (stepX < 0 && cell.wallWest)
        hitWall = true;
    } else {
      // Crossed a horizontal line (North or South wall)
      if (stepY > 0 && cell.wallSouth)
        hitWall = true;
      else if (stepY < 0 && cell.wallNorth)
        hitWall = true;
    }
  }

  // 6. Calculate exact hit coordinate
  float perpWallDist =
      (hitSide == 0) ? (sideDist.x - deltaDist.x) : (sideDist.y - deltaDist.y);
  glm::vec2 hitPos = startPos + dir * perpWallDist;

  // Nudge slightly away from the wall along the inverse normal to prevent
  // floating-point self-intersection
  glm::vec2 normal = {0.0f, 0.0f};
  if (hitSide == 0) {
    normal.x = (stepX > 0) ? -1.0f : 1.0f;
  } else {
    normal.y = (stepY > 0) ? -1.0f : 1.0f;
  }
  glm::vec2 offsetHitPos = hitPos - (normal * 0.001f);

  // 7. Calculate reflection angle
  // Standard 2D reflection formula: R = D - 2(D · N)N
  glm::vec2 reflectDir = dir - 2.0f * glm::dot(dir, normal) * normal;
  float newAngle = std::atan2(reflectDir.y, reflectDir.x);

  return {offsetHitPos, newAngle};
}
