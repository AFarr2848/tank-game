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

  glm::vec2 normalizedCenter() {
    return center * glm::vec2(0.5) + glm::vec2(0.5);
  }

  std::array<GLfloat, 9> getTransformationFloats() const {
    glm::mat3 transform = getTransformation();
    std::array<GLfloat, 9> floats;
    std::copy_n(glm::value_ptr(transform), 9, floats.begin());
    return floats;
  }

  glm::mat3 getTransformation() const {
    glm::mat3 T = glm::translate(glm::mat3(1.0f), center);
    glm::mat3 R = glm::rotate(glm::mat3(1.0f), rotation);
    glm::mat3 S = glm::scale(glm::mat3(1.0f), size * 0.5f);
    return T * R * S;
  }

  bool contains(glm::vec2 point) const {
    glm::vec3 localPoint =
        glm::inverse(getTransformation()) * glm::vec3(point, 1.0f);
    return localPoint.x >= -1.0f && localPoint.x <= 1.0f &&
           localPoint.y >= -1.0f && localPoint.y <= 1.0f;
  }

  std::vector<glm::vec2> intersectLine(
      std::pair<glm::vec2, glm::vec2> line) const {
    glm::mat3 invTrans = glm::inverse(getTransformation());
    glm::mat3 trans = getTransformation();

    auto transformPoint = [](const glm::mat3& m, glm::vec2 p) {
      glm::vec3 tp = m * glm::vec3(p, 1.0f);
      return glm::vec2(tp.x / tp.z, tp.y / tp.z);
    };

    glm::vec2 p1 = transformPoint(invTrans, line.first);
    glm::vec2 p2 = transformPoint(invTrans, line.second);
    glm::vec2 d = p2 - p1;

    std::vector<glm::vec2> intersections;
    constexpr float EPSILON = 1e-4f;

    auto tryAddIntersection = [&](float t, float x, float y) {
      if (t >= -EPSILON && t <= 1.0f + EPSILON && x >= -1.0f - EPSILON &&
          x <= 1.0f + EPSILON && y >= -1.0f - EPSILON && y <= 1.0f + EPSILON) {
        x = glm::clamp(x, -1.0f, 1.0f);
        y = glm::clamp(y, -1.0f, 1.0f);

        glm::vec2 worldPt = transformPoint(trans, glm::vec2(x, y));

        for (const auto& pt : intersections) {
          if (glm::distance(pt, worldPt) < EPSILON)
            return;
        }
        intersections.push_back(worldPt);
      }
    };

    if (std::abs(d.x) > EPSILON) {
      for (float xBoundary : {-1.0f, 1.0f}) {
        float t = (xBoundary - p1.x) / d.x;
        float y = p1.y + t * d.y;
        tryAddIntersection(t, xBoundary, y);
      }
    }
    if (std::abs(d.y) > EPSILON) {
      for (float yBoundary : {-1.0f, 1.0f}) {
        float t = (yBoundary - p1.y) / d.y;
        float x = p1.x + t * d.x;
        tryAddIntersection(t, x, yBoundary);
      }
    }

    std::cout << intersections.size() << std::endl;
    return intersections;
  }
};

}  // namespace Tnk
