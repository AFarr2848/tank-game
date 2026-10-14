#pragma once
#include <GL/gl.h>
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Tnk {
const int MAX_LIGHTS = 64;

struct PointLight {
  glm::vec2 location;
  float radius;
  float _pad0;
  glm::vec3 color;
  float _pad1;
};

struct CascadeInfo {
  int mazeSize;
  glm::vec2 screenSize;
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  glm::vec3 backgroundColor;
  int numLights;
};
struct CascadeInfoLocations {
  GLint mazeSize;
  GLint screenSize;
  GLint cascadeLevel;
  GLint linearInterval;
  GLint rayInterval;
  GLint backgroundColor;
  GLint numLights;
  void init(GLuint shaderProgram);
};

}  // namespace Tnk
