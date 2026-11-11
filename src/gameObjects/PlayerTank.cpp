#include "tank-game/gameObjects/PlayerTank.hpp"
#include <GLFW/glfw3.h>
#include "tank-game/InputHelper.hpp"
using namespace Tnk;

void PlayerTank::updatePlayer(float deltaTime,
                              InputHelper& inputHelper,
                              Maze& maze,
                              std::vector<Bullet>& bulletVec) {
  update(deltaTime);
  updateInputs(deltaTime, inputHelper, maze, bulletVec);
}

void PlayerTank::update(float deltaTime) {
  sprite.bounds = bounds;
  mathTimer += deltaTime;
  timeSinceFired += deltaTime;
}

glm::vec2 PlayerTank::checkMoveCollision(float move, Maze& maze) {
  glm::vec2 moveVec = glm::vec2(std::cos(bounds.rotation) * move,
                                std::sin(bounds.rotation) * move);

  Rect nextBounds = Rect(bounds);
  nextBounds.center += moveVec;

  for (auto colDir : bounds.collideMaze(maze)) {
    if (colDir == CollisionDirection::DIR_NORTH && moveVec.y > 0)
      moveVec.y = 0;
    if (colDir == CollisionDirection::DIR_SOUTH && moveVec.y < 0)
      moveVec.y = 0;
    if (colDir == CollisionDirection::DIR_EAST && moveVec.x > 0)
      moveVec.x = 0;
    if (colDir == CollisionDirection::DIR_WEST && moveVec.x < 0)
      moveVec.x = 0;
  }
  return moveVec;
}

void PlayerTank::shoot(std::vector<Bullet>& bulletVec) {
  if (currentBullets < maxBullets) {
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
    bullet.vel = bounds.getFront() * bulletSpeed;
    bulletVec.push_back(bullet);
  }
}

void PlayerTank::updateInputs(float deltaTime,
                              InputHelper& inputHelper,
                              Maze& maze,
                              std::vector<Bullet>& bulletVec) {
  auto isKeyDown = [this, &inputHelper](int key) {
    return inputHelper.isKeyDown(key);
  };

  if (isKeyDown(GLFW_KEY_W))
    moveTank(checkMoveCollision(deltaTime * moveSpeed, maze));
  if (isKeyDown(GLFW_KEY_S))
    moveTank(checkMoveCollision(-deltaTime * moveSpeed, maze));
  if (isKeyDown(GLFW_KEY_A))
    turnTank(deltaTime * turnSpeed);
  if (isKeyDown(GLFW_KEY_D))
    turnTank(-deltaTime * turnSpeed);
  if (inputHelper.isKeyDownToggle(GLFW_KEY_SPACE))
    shoot(bulletVec);
}
