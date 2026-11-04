#include "tank-game/gameObjects/EnemyTank.hpp"
using namespace Tnk;
void EnemyTank::updateAI() {}
void EnemyTank::update(float deltaTime) {
  sprite.bounds = bounds;
}
