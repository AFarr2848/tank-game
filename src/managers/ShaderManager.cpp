#include "tank-game/managers/ShaderManager.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

GLuint tnk_ShaderManager::compileShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  return shader;
}

std::vector<GLuint> tnk_ShaderManager::loadVertAndFragShaders(
    std::filesystem::path vertexPath,
    std::filesystem::path fragPath) {
  std::vector<GLuint> programs;
  std::ifstream vertFile(vertexPath, std::ios::binary);
  std::ifstream fragFile(fragPath, std::ios::binary);

  if (!vertFile.is_open() || !fragFile.is_open()) {
    throw std::runtime_error("Shader file not found!");
  }

  std::string vertContents((std::istreambuf_iterator<char>(vertFile)),
                           std::istreambuf_iterator<char>());
  std::string fragContents((std::istreambuf_iterator<char>(fragFile)),
                           std::istreambuf_iterator<char>());

  GLuint vs = compileShader(GL_VERTEX_SHADER, vertContents.c_str());
  GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragContents.c_str());

  return {vs, fs};
}

GLuint tnk_ShaderManager::loadCompShader(std::filesystem::path compPath) {
  std::vector<GLuint> programs;
  std::ifstream compFile(compPath, std::ios::binary);

  if (!compFile.is_open()) {
    throw std::runtime_error("Shader file not found!");
  }

  std::string compContents((std::istreambuf_iterator<char>(compFile)),
                           std::istreambuf_iterator<char>());

  GLuint cs = compileShader(GL_VERTEX_SHADER, compContents.c_str());

  return cs;
}

void tnk_ShaderManager::loadShaders() {
  auto triangleShaders = loadVertAndFragShaders("/shaders/triangle.vert",
                                                "/shaders/triangle.frag");
  shaderProgramMap["triangleProgram"] = glCreateProgram();
  glAttachShader(shaderProgramMap["triangleProgram"], triangleShaders[0]);
  glAttachShader(shaderProgramMap["triangleProgram"], triangleShaders[1]);
  glLinkProgram(shaderProgramMap["triangleProgram"]);
}
