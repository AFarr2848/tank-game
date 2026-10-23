#pragma once
#include <GLES3/gl3.h>
#include <filesystem>
#include <unordered_map>
#include "glm/ext/vector_float2.hpp"
namespace Tnk {
class Maze;
struct Texture {
  GLuint texID;
};

class TextureManager {
 public:
  GLuint fbo;
  Texture get(std::string name);
  void makeTextures(int screenW, int screenH);
  void makeBufferTextures(int screenW, int screenH);
  void makeWallTex(Maze& maze);
  void makeLightingTextures(glm::vec2 texSize);

 private:
  std::unordered_map<std::string, Texture> texMap;
  void makeBlankTexture(std::string name,
                        int screenW,
                        int screenH,
                        GLint internalFormat,
                        GLenum format,
                        GLenum samplingMethod);
  void makeTextureFromImage(std::string name, std::filesystem::path imagePath);
};

}  // namespace Tnk
