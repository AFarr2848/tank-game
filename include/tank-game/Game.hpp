#pragma once
#include "tank-game/gameObjects/Maze.hpp"
#include "tank-game/gameObjects/Tank.hpp"

namespace Tnk {
class InputHelper;
struct TimingData;
class Game {
 public:
  Game(InputHelper& inputHelper, TimingData& timingData)
      : inputHelper(inputHelper), timingData(timingData) {};
  void startGame();
  void getMazeLines();
  std::vector<glm::vec2> mazeLines;
  Tank playerTank;
  void getInputs();

 private:
  Maze maze;
  InputHelper& inputHelper;
  TimingData& timingData;

  void movePlayer();
};

}  // namespace Tnk
