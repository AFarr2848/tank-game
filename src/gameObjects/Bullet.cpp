#include "tank-game/gameObjects/Bullet.hpp"
#include "tank-game/Geometry.hpp"
#include "tank-game/gameObjects/Tank.hpp"
using namespace Tnk;

void Bullet::moveBullet(float deltaTime, Maze& maze) {
  bounds.center += vel * deltaTime;
  for (auto colDir : bounds.collideMaze(maze)) {
    if (colDir == CollisionDirection::DIR_NORTH && vel.y > 0) {
      vel.y *= -1;
      bounces += 1;
    }
    if (colDir == CollisionDirection::DIR_SOUTH && vel.y < 0) {
      vel.y *= -1;
      bounces += 1;
    }
    if (colDir == CollisionDirection::DIR_EAST && vel.x > 0) {
      vel.x *= -1;
      bounces += 1;
    }
    if (colDir == CollisionDirection::DIR_WEST && vel.x < 0) {
      vel.x *= -1;
      bounces += 1;
    }
  }
}
