#include "tank-game/Game.hpp"
using namespace Tnk;
void Game::startGame() {
  maze.makeMaze(4);
  getMazeLines();
}

void Game::getMazeLines() {
  mazeLines = maze.makeLines();
}
