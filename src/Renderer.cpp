#include "tank-game/Renderer.hpp"
#include <GLES3/gl3.h>
#include <iostream>
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
