#include "tank-game/Lighting.hpp"
#include <GLES3/gl3.h>
#include <cmath>
#include <numbers>
using namespace Tnk;

void Lighting::incrementCascadeUniforms(int cascadeLevel) {
  cascadeInfo.cascadeLevel = cascadeLevel;
  cascadeInfo.linearInterval = 2.0 * exp2(float(cascadeInfo.cascadeLevel));
  cascadeInfo.rayInterval =
      2.0 * std::numbers::pi /
      (cascadeInfo.linearInterval * cascadeInfo.linearInterval);
};
void Lighting::incrementMergeUniforms(int cascadeLevel) {
  mergeInfo.cascadeLevel = cascadeLevel;
  mergeInfo.linearInterval = 2.0 * exp2(float(mergeInfo.cascadeLevel));
  mergeInfo.rayInterval = 2.0 * std::numbers::pi /
                          (mergeInfo.linearInterval * mergeInfo.linearInterval);
}

void Lighting::setMazeBakeUniforms() {
  glUniform1i(bakeLocations.mazeSize, bakeInfo.mazeSize);
  glUniform1i(bakeLocations.numCascades, MAX_CASCADES);
  glUniform2f(bakeLocations.screenSize, bakeInfo.screenSize.x,
              bakeInfo.screenSize.y);
}
void Lighting::initMazeBakeLocations(GLuint shaderProgram) {
  bakeLocations.mazeSize =
      glGetUniformLocation(shaderProgram, "mazeInfo.mazeSize");
  bakeLocations.screenSize =
      glGetUniformLocation(shaderProgram, "mazeInfo.screenSize");
  bakeLocations.numCascades =
      glGetUniformLocation(shaderProgram, "mazeInfo.numCascades");
}

void Lighting::setCascadeInfoUniforms() {
  glUniform1i(cascadeLocations.mazeSize, cascadeInfo.mazeSize);
  glUniform2f(cascadeLocations.screenSize, cascadeInfo.screenSize.x,
              cascadeInfo.screenSize.y);
  glUniform1i(cascadeLocations.cascadeLevel, cascadeInfo.cascadeLevel);
  glUniform1f(cascadeLocations.linearInterval, cascadeInfo.linearInterval);
  glUniform1f(cascadeLocations.rayInterval, cascadeInfo.rayInterval);
  glUniform1i(cascadeLocations.numLights, cascadeInfo.numLights);
}
void Lighting::setMergeInfoUniforms() {
  glUniform2f(mergeLocations.screenSize, mergeInfo.screenSize.x,
              mergeInfo.screenSize.y);
  glUniform1i(mergeLocations.cascadeLevel, mergeInfo.cascadeLevel);
  glUniform1f(mergeLocations.linearInterval, mergeInfo.linearInterval);
  glUniform1f(mergeLocations.rayInterval, mergeInfo.rayInterval);
}

void Lighting::initLocations(GLuint cascadeShader,
                             GLuint mergeShader,
                             GLuint bakeShader,
                             GLuint averageShader) {
  initCascadeInfoLocations(cascadeShader);
  initMergeInfoLocations(mergeShader);
  initMazeBakeLocations(bakeShader);
  average_screenSizeLocation =
      glGetUniformLocation(averageShader, "screenSize");
}

void Lighting::initCascadeInfoLocations(GLuint shaderProgram) {
  cascadeLocations.mazeSize =
      glGetUniformLocation(shaderProgram, "cascadeInfo.mazeSize");
  cascadeLocations.screenSize =
      glGetUniformLocation(shaderProgram, "cascadeInfo.screenSize");
  cascadeLocations.cascadeLevel =
      glGetUniformLocation(shaderProgram, "cascadeInfo.cascadeLevel");
  cascadeLocations.linearInterval =
      glGetUniformLocation(shaderProgram, "cascadeInfo.linearInterval");
  cascadeLocations.rayInterval =
      glGetUniformLocation(shaderProgram, "cascadeInfo.rayInterval");
  cascadeLocations.numLights =
      glGetUniformLocation(shaderProgram, "cascadeInfo.numLights");
}

void Lighting::initMergeInfoLocations(GLuint shaderProgram) {
  mergeLocations.screenSize =
      glGetUniformLocation(shaderProgram, "mergeInfo.screenSize");
  mergeLocations.cascadeLevel =
      glGetUniformLocation(shaderProgram, "mergeInfo.cascadeLevel");
  mergeLocations.linearInterval =
      glGetUniformLocation(shaderProgram, "mergeInfo.linearInterval");
  mergeLocations.rayInterval =
      glGetUniformLocation(shaderProgram, "mergeInfo.rayInterval");
}
