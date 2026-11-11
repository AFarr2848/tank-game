#include "tank-game/gameObjects/SpecialTanks.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include "tank-game/Random.hpp"
using namespace Tnk;

void EnemyTankSprayer::shoot(std::vector<Bullet>& bulletVec) {
  requestShoot = false;
  timeSinceFired = 0;
  currentBullets++;
  Bullet bullet(
      Rect{

          .center = bounds.center +
                    (bounds.getFront() * bounds.size.y / glm::vec2(2)),
          .size = glm::vec2(0.03),
          .rotation = 0

      },
      this->id, this->maxRicochets);
  float offset = Random::getReal(-std::numbers::pi / 8, std::numbers::pi / 8);

  bullet.vel = (glm::rotate(bounds.getFront(), offset)) * bulletSpeed;
  bulletVec.push_back(bullet);
}

void EnemyTankChaser::update(float deltaTime) {
  sprite.bounds = bounds;
  tankAI.thinkTimer += deltaTime;
  moveToTarget(deltaTime);
}
