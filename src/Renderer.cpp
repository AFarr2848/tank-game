#include "tank-game/Renderer.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLES3/gl3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>
#include "glm/gtc/type_ptr.hpp"
#include "tank-game/Lighting.hpp"
#include "tank-game/Window.hpp"
#include "tank-game/managers/BufferManager.hpp"
#include "tank-game/managers/ShaderManager.hpp"
#include "tank-game/managers/TextureManager.hpp"
using namespace Tnk;

void Renderer::drawScreen() {
  GLuint emptyVAO;
  glGenVertexArrays(1, &emptyVAO);

  glUseProgram(shaderMan.shaderProgramMap["triangleProgram"]);
  glBindVertexArray(emptyVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::drawMaze(const std::vector<glm::vec2>& lines) {
  glEnable(GL_BLEND);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texMan.get("mazeTex").texID, 0);

  glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  bufferMan.get("lineBuffer").bind();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);

  glUseProgram(shaderMan.shaderProgramMap["mazeProgram"]);

  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size()));
}

void Renderer::drawSprites(const std::vector<Sprite>& sprites) {
  glEnable(GL_BLEND);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texMan.get("mazeTex").texID, 0);
  glUseProgram(shaderMan.shaderProgramMap["spriteProgram"]);

  bufferMan.get("spriteBuffer").bind();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);
  GLint shaderLocation = glGetUniformLocation(
      shaderMan.shaderProgramMap["spriteProgram"], "transform");
  for (auto& sprite : sprites) {
    glBindTexture(GL_TEXTURE_2D, texMan.get(sprite.texture).texID);

    glUniformMatrix3fv(shaderLocation, 1, GL_FALSE,
                       glm::value_ptr(sprite.bounds.getTransformation()));

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void Renderer::drawCascades(std::array<PointLight, MAX_LIGHTS>& lights) {
  glDisable(GL_BLEND);
  glBindFramebuffer(GL_FRAMEBUFFER, texMan.fbo);
  GLuint program = shaderMan.shaderProgramMap["cascadeProgram"];
  glUseProgram(program);
  bufferMan.updateBuffer("lightBuffer", lights.data(),
                         sizeof(PointLight) * MAX_LIGHTS);
  bufferMan.get("lightBuffer").bind();
  glBindTexture(GL_TEXTURE_2D, texMan.get("mazeBakeTex").texID);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferMan.get("lightBuffer").bufferID);
  bufferMan.get("emptyBuffer").bind();
  for (int i = 0; i < MAX_CASCADES; i++) {
    lighting.incrementCascadeUniforms(i);
    lighting.setCascadeInfoUniforms();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texMan.get("cascade" + std::to_string(i)).texID, 0);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void Renderer::mergeCascades() {
  glDisable(GL_BLEND);
  glBindFramebuffer(GL_FRAMEBUFFER, texMan.fbo);
  GLuint program = shaderMan.shaderProgramMap["mergeProgram"];
  glUseProgram(program);
  glUniform1i(glGetUniformLocation(shaderMan.shaderProgramMap["mergeProgram"],
                                   "highCascade"),
              0);
  glUniform1i(glGetUniformLocation(shaderMan.shaderProgramMap["mergeProgram"],
                                   "lowCascade"),
              1);

  std::string currentReadTex = "cascade3";
  for (int i = MAX_CASCADES - 1; i > 0; i--) {
    lighting.incrementMergeUniforms(i - 1);
    std::string currentFBTex =
        (currentReadTex == "lightingTex") ? "tempLightingTex" : "lightingTex";
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texMan.get(currentFBTex).texID, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,
                  texMan.get("cascade" + std::to_string(i - 1)).texID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texMan.get(currentReadTex).texID);
    lighting.setMergeInfoUniforms();

    // std::cout << "Merge pass " + std::to_string(i) +
    //                  ": Higher= " + currentReadTex +
    //                  ", Lower= " + std::to_string(i - 1) +
    //                  ", FB= " + currentFBTex
    // << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, 6);
    currentReadTex = currentFBTex;
  }
}

void Renderer::combineLighting() {
  glEnable(GL_BLEND);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLuint shader = shaderMan.shaderProgramMap["averageProgram"];
  glUseProgram(shader);

  glUniform1i(glGetUniformLocation(shaderMan.shaderProgramMap["averageProgram"],
                                   "screenTex"),
              0);
  glUniform1i(glGetUniformLocation(shaderMan.shaderProgramMap["averageProgram"],
                                   "lightingTex"),
              1);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texMan.get("lightingTex").texID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texMan.get("mazeTex").texID);

  glUniform1f(lighting.average_resolutionLocation, lighting.lightingResolution);

  bufferMan.get("emptyBuffer").bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawImage() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLuint shader = shaderMan.shaderProgramMap["imageProgram"];
  glUseProgram(shader);
  bufferMan.get("emptyBuffer").bind();
  glBindTexture(GL_TEXTURE_2D, texMan.get("lightingTex").texID);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::bakeMazeCollisions() {
  glDisable(GL_BLEND);
  glBindFramebuffer(GL_FRAMEBUFFER, texMan.fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texMan.get("mazeBakeTex").texID, 0);
  GLuint shader = shaderMan.shaderProgramMap["mazeBakeProgram"];
  glUseProgram(shader);
  bufferMan.get("emptyBuffer").bind();
  lighting.setMazeBakeUniforms();
  glBindTexture(GL_TEXTURE_2D, texMan.get("wallTex").texID);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
