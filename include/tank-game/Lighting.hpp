#pragma once
#include <GL/gl.h>
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Tnk {
const int MAX_LIGHTS = 64;
const int INTERVAL_INCREASE_FACTOR = 4;
const int MAX_CASCADES = 4;

struct PointLight {
  glm::vec2 location;
  float radius;
  float _pad0;
  glm::vec3 color;
  float _pad1;
};

struct CascadeInfo {
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  int mazeSize;
  glm::vec2 screenSize;
  int numLights;
};
struct MergeInfo {
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  glm::vec2 screenSize;
};
struct MazeBakeInfo {
  int mazeSize;
  glm::vec2 screenSize;
  int numCascades;
};

struct CascadeInfoLocations {
  GLint cascadeLevel;
  GLint linearInterval;
  GLint rayInterval;
  GLint mazeSize;
  GLint screenSize;
  GLint numLights;
};
struct MergeInfoLocations {
  GLint cascadeLevel;
  GLint linearInterval;
  GLint rayInterval;
  GLint screenSize;
};
struct MazeBakeLocations {
  GLint mazeSize;
  GLint screenSize;
  GLint numCascades;
};

class Lighting {
 public:
  void initLocations(GLuint cascadeShader,
                     GLuint mergeShader,
                     GLuint bakeShader,
                     GLuint averageShader);
  void preloadMazeCollisions();

  void setCascadeInfoUniforms();
  void setMazeBakeUniforms();
  void initCascadeInfoLocations(GLuint shaderProgram);
  void initMergeInfoLocations(GLuint shaderProgram);
  void initMazeBakeLocations(GLuint shaderProgram);
  void setMergeInfoUniforms();
  void incrementMergeUniforms(int cascadeLevel);
  void incrementCascadeUniforms(int cascadeLevel);
  CascadeInfo cascadeInfo;
  MergeInfo mergeInfo;
  MazeBakeInfo bakeInfo;
  GLint average_screenSizeLocation;

 private:
  CascadeInfoLocations cascadeLocations;
  MergeInfoLocations mergeLocations;
  MazeBakeLocations bakeLocations;
};

}  // namespace Tnk
