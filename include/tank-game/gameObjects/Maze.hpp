#pragma once
#include <random>
#include <vector>
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"

namespace Tnk {

class Maze {
 public:
  struct MazeCell {
    bool wallNorth = true, wallSouth = true, wallEast = true, wallWest = true;
    bool visited = false;
  };
  struct RayBounceResult {
    glm::vec2 pos;
    float angle;
  };

  int mazeSize;
  std::vector<std::vector<MazeCell>> cells;

  void makeMaze(int mazeSize);
  std::vector<glm::vec2> makeLines();
  std::vector<glm::ivec2> getConnectedCells(glm::ivec2 cellCoords);
  glm::vec2 mazeToNdcCoords(glm::ivec2 mazeCoords);
  glm::ivec2 NDCtoMazeCoords(glm::vec2 pos);
  RayBounceResult calculateRayBounce(glm::vec2 startPos, float angle);

 private:
  std::random_device rd;
  std::mt19937 gen;

  void carveFrom(glm::vec2 cellCoords);
  bool canCarveInDirection(glm::vec2 cellCoords, glm::vec2 dir);
  void changeWalls(glm::vec2 cellCoords, glm::vec2 carveDir);
};
}  // namespace Tnk
