#include "tank-game/gameObjects/Maze.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>
using namespace Tnk;

std::vector<glm::vec2> Maze::makeLines() {
  std::vector<glm::vec2> lines;
  lines.reserve(mazeSize * mazeSize * 4);

  auto toNDC = [this](float x, float y) -> glm::vec2 {
    float scale = 2.0f / mazeSize;
    return {(x * scale) - 1.0f, 1.0f - (y * scale)};
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
