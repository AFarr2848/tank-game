#pragma once

#include <string>
#include "tank-game/Geometry.hpp"
namespace Tnk {
struct Sprite {
  Sprite(Rect bounds) : bounds(bounds) {}
  Sprite() {
    bounds = Rect({.center = {0, 0}, .size = {0.5, 0.5}, .rotation = 0});
  }

  Rect bounds;
  std::string texture;
  std::vector<glm::vec2> getVertices() {
    std::vector<glm::vec2> rectCoords = {

        // triangle 1
        {1, 1},    //
        {1, -1},   //
        {-1, -1},  //

        // triangle 2
        {-1, 1},  //
        {1, 1},   //
        {-1, -1}  //

    };
    return rectCoords;
  }
};
}  // namespace Tnk
