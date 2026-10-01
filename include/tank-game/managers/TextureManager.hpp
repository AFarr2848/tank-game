#pragma once
#include <GLES3/gl3.h>
#include <filesystem>
#include <unordered_map>
namespace Tnk {
struct Texture {
  GLuint texID;
};

class TextureManager {
 public:
  void makeTexture(std::string name, std::filesystem::path imagePath);
  Texture get(std::string name);
  void makeTextures();

 private:
  std::unordered_map<std::string, Texture> texMap;
};

}  // namespace Tnk
