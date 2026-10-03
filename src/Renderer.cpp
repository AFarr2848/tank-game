#include "tank-game/Renderer.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "tank-game/managers/BufferManager.hpp"
#include "tank-game/managers/ShaderManager.hpp"
#include "tank-game/managers/TextureManager.hpp"
using namespace Tnk;

void Renderer::drawScreen() {
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  GLuint emptyVAO;
  glGenVertexArrays(1, &emptyVAO);

  glUseProgram(shaderMan.shaderProgramMap["triangleProgram"]);
  glBindVertexArray(emptyVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::drawMaze(const std::vector<glm::vec2>& lines) {
  bufferMan.get("lineBuffer").bind();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);

  glUseProgram(shaderMan.shaderProgramMap["mazeProgram"]);

  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size()));
}

void Renderer::drawSprites(const std::vector<Sprite>& sprites) {
  glUseProgram(shaderMan.shaderProgramMap["spriteProgram"]);

  bufferMan.get("spriteBuffer").bind();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);
  for (auto& sprite : sprites) {
    std::cout << glm::to_string(sprite.bounds->getTransformation())
              << std::endl;
    glBindTexture(GL_TEXTURE_2D, texMan.get(sprite.texture).texID);

    glUniformMatrix3fv(
        glGetUniformLocation(shaderMan.shaderProgramMap["spriteProgram"],
                             "transform"),
        1, GL_FALSE, glm::value_ptr(sprite.bounds->getTransformation()));

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}
