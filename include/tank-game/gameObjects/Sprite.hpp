#pragma once

#include <string>
#include "tank-game/Geometry.hpp"
namespace Tnk {
struct Sprite {
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
