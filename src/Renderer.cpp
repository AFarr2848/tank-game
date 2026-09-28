#include "tank-game/Renderer.hpp"
#include <GLES3/gl3.h>
#include <iostream>
#include "glm/ext/vector_float4.hpp"
#include "tank-game/managers/BufferManager.hpp"
#include "tank-game/managers/ShaderManager.hpp"
using namespace Tnk;

void Renderer::drawScreen() {
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  GLuint emptyVAO;
  glGenVertexArrays(1, &emptyVAO);

  glUseProgram(shaderMan.shaderProgramMap["triangleProgram"]);
  glBindVertexArray(emptyVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  std::cout << "Screen draw" << std::endl;
}

void Renderer::drawMaze(const std::vector<glm::vec2>& lines) {
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
  glEnableVertexAttribArray(0);

  glUseProgram(shaderMan.shaderProgramMap["mazeProgram"]);

  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size()));
}
