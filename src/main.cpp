#include <iostream>
#include "tank-game/Engine.hpp"

int main() {
  try {
    tnk_Engine engine{};
    engine.startEngine();
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
  }
}
