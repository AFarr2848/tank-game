#pragma once
#include <GL/gl.h>
#include <array>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "glm/ext/vector_float2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
namespace Tnk {
class Maze;

enum struct CollisionDirection {
  DIR_NONE,
  DIR_NORTH,
  DIR_SOUTH,
  DIR_EAST,
  DIR_WEST
};

/**
 * @class Rect
 * @brief A 2d rectangle defined by a 2 x 2 box around a center point that is
 * then transformed by a matrix
 */
struct Rect {
  glm::vec2 center{0.0f};
  glm::vec2 size{1.0f};
  float rotation{0.0f};

  glm::vec2 normalizedCenter() {
    return center * glm::vec2(0.5) + glm::vec2(0.5);
  }

  std::array<GLfloat, 9> getTransformationFloats() const {
    glm::mat3 transform = getTransformation();
    std::array<GLfloat, 9> floats;
    std::copy_n(glm::value_ptr(transform), 9, floats.begin());
    return floats;
  }

  glm::mat3 getTransformation() const;

  bool contains(glm::vec2 point) const {
    glm::vec3 localPoint =
        glm::inverse(getTransformation()) * glm::vec3(point, 1.0f);
    return localPoint.x >= -1.0f && localPoint.x <= 1.0f &&
           localPoint.y >= -1.0f && localPoint.y <= 1.0f;
  }

  std::vector<CollisionDirection> collideMaze(Maze& maze);
};

}  // namespace Tnk
//
