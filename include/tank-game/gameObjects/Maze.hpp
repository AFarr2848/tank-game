#pragma once
#include <random>
#include <vector>
#include "glm/glm.hpp"

namespace Tnk {

class Maze {
 public:
  struct MazeCell {
    bool wallNorth = true, wallSouth = true, wallEast = true, wallWest = true;
    bool visited = false;
  };
  int mazeSize;
  std::vector<std::vector<MazeCell>> cells;

  void makeMaze(int mazeSize);
  std::vector<glm::vec2> makeLines();

 private:
  std::random_device rd;
  std::mt19937 gen;

  void carveFrom(glm::vec2 cellCoords);
  bool canCarveInDirection(glm::vec2 cellCoords, glm::vec2 dir);
  void changeWalls(glm::vec2 cellCoords, glm::vec2 carveDir);
};
}  // namespace Tnk
