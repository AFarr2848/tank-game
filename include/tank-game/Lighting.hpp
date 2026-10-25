#pragma once
#include <GL/gl.h>
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Tnk {
const int MAX_LIGHTS = 64;
const int INTERVAL_INCREASE_FACTOR = 4;
const int MAX_CASCADES = 4;
// doesnt work
const float INITIAL_LINEAR_INTERVAL = 2.0;

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
  float resolution;
};
struct MergeInfo {
  int cascadeLevel;
  float linearInterval;
  float rayInterval;
  glm::vec2 screenSize;
  float resolution;
};
struct MazeBakeInfo {
  int mazeSize;
  glm::vec2 screenSize;
  int numCascades;
  float resolution;
};

struct CascadeInfoLocations {
  GLint cascadeLevel;
  GLint linearInterval;
  GLint rayInterval;
  GLint mazeSize;
  GLint screenSize;
  GLint numLights;
  GLint resolution;
};
struct MergeInfoLocations {
  GLint cascadeLevel;
  GLint linearInterval;
  GLint rayInterval;
  GLint screenSize;
  GLint resolution;
};
struct MazeBakeLocations {
  GLint mazeSize;
  GLint screenSize;
  GLint numCascades;
  GLint initialLinearInterval;
  GLint resolution;
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
  GLint average_resolutionLocation;
  float lightingResolution = 0.5;

 private:
  CascadeInfoLocations cascadeLocations;
  MergeInfoLocations mergeLocations;
  MazeBakeLocations bakeLocations;
};

}  // namespace Tnk
