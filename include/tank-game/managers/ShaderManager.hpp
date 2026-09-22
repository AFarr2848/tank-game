#pragma once
#include <GLES3/gl3.h>
#include <filesystem>
#include <map>
#include <vector>

namespace Tnk {

class ShaderManager {
 public:
  void loadShaders();

  std::map<std::string, GLuint> shaderProgramMap;

 private:
  GLuint compileShader(GLenum type, const char* source);
  std::vector<GLuint> loadVertAndFragShaders(std::filesystem::path vertexPath,
                                             std::filesystem::path fragPath);
  GLuint loadCompShader(std::filesystem::path compPath);
};
}  // namespace Tnk
