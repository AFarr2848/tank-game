#include <iostream>
#include "tank-game/Engine.hpp"
using namespace Tnk;

int main() {
  try {
    Engine engine{};
    engine.startEngine();
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
  }
}
