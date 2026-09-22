#include "tank-game/gameObjects/Maze.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <random>
#include <stdexcept>
#include <vector>
using namespace Tnk;

std::vector<glm::vec2> Maze::makeLines() {}

bool Maze::canCarveInDirection(glm::vec2 cellCoords, glm::vec2 dir) {
  glm::vec2 mazeCoords = cellCoords + dir;
  if (mazeCoords.x >= mazeSize || mazeCoords.x < 0 ||
      mazeCoords.y >= mazeSize || mazeCoords.y < 0)
    return false;
  return !cells.at(mazeCoords.y).at(mazeCoords.x).visited;
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
  gen = std::mt19937(rd());
  uniform_int_distribution<> distrib(0, mazeSize - 1);

  glm::vec2 startingCell = {distrib(gen), distrib(gen)};
  carveFrom(startingCell);
}
