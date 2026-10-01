#pragma once
#include <GL/gl.h>
#include <array>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <iostream>
#include "glm/ext/vector_float2.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
namespace Tnk {

/**
 * @class Rect
 * @brief A 2d rectangle defined by a 2 x 2 box around a center point that is
 * then transformed by a matrix
 */
struct Rect {
  glm::vec2 center{0.0f};
  glm::vec2 size{1.0f};
  float rotation{0.0f};

  std::array<GLfloat, 9> getTransformationFloats() const {
    glm::mat3 transform = getTransformation();
    std::array<GLfloat, 9> floats;
    std::copy_n(glm::value_ptr(transform), 9, floats.begin());
    return floats;
  }

  glm::mat3 getTransformation() const {
    glm::mat3 T = glm::translate(glm::mat3(1.0f), center);
    glm::mat3 R = glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                              glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat3 S = glm::scale(glm::mat3(1.0f), size * 0.5f);
    return T * R * S;
  }

  bool contains(glm::vec2 point) const {
    glm::vec3 localPoint =
        glm::inverse(getTransformation()) * glm::vec3(point, 1.0f);
    return localPoint.x >= -1.0f && localPoint.x <= 1.0f &&
           localPoint.y >= -1.0f && localPoint.y <= 1.0f;
  }

  // I got some help on this one lol
  std::vector<glm::vec2> intersectLine(
      std::pair<glm::vec2, glm::vec2> line) const {
    glm::mat3 invTrans = glm::inverse(getTransformation());
    glm::mat3 trans = getTransformation();

    glm::vec2 p1 = glm::vec2(invTrans * glm::vec3(line.first, 1.0f));
    glm::vec2 p2 = glm::vec2(invTrans * glm::vec3(line.second, 1.0f));
    glm::vec2 d = p2 - p1;

    std::vector<glm::vec2> intersections;

    auto tryAddIntersection = [&](float t, float x, float y) {
      if (t >= 0.0f && t <= 1.0f && x >= -1.0f && x <= 1.0f && y >= -1.0f &&
          y <= 1.0f) {
        glm::vec2 worldPt = glm::vec2(trans * glm::vec3(x, y, 1.0f));
        intersections.push_back(worldPt);
      }
    };

    if (d.x != 0.0f) {
      for (float xBoundary : {-1.0f, 1.0f}) {
        float t = (xBoundary - p1.x) / d.x;
        float y = p1.y + t * d.y;
        tryAddIntersection(t, xBoundary, y);
      }
    }
    if (d.y != 0.0f) {
      for (float yBoundary : {-1.0f, 1.0f}) {
        float t = (yBoundary - p1.y) / d.y;
        float x = p1.x + t * d.x;
        tryAddIntersection(t, x, yBoundary);
      }
    }

    return intersections;
  }
};

}  // namespace Tnk
