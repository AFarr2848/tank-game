#pragma once
#include "tank-game/gameObjects/Maze.hpp"
namespace Tnk {
class Game {
 public:
  void startGame();
  void getMazeLines();
  std::vector<glm::vec2> mazeLines;

 private:
  Maze maze;
};

}  // namespace Tnk
