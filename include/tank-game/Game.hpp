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
  void updateGame();
  int mazeSize;
  int numLights;
  Maze maze;

  std::vector<Sprite> getSprites();
  std::array<PointLight, MAX_LIGHTS> getLights();

 private:
  InputHelper& inputHelper;
  TimingData& timingData;

  void getInputs();

  std::vector<Bullet> bulletVec;
};

}  // namespace Tnk
