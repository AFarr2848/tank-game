
#include "tank-game/managers/TextureManager.hpp"
#include <GL/gl.h>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Tnk;

Texture TextureManager::get(std::string name) {
  try {
    return texMap.at(name);

  } catch (std::out_of_range& e) {
    throw std::runtime_error("Texture '" + name + "' not found!");
  }
}

void TextureManager::makeTexture(std::string name,
                                 const std::filesystem::path imagePath) {
  int width, height, nrChannels;

  // Force stbi_load to convert whatever it finds into 4-channel RGBA
  unsigned char* data = stbi_load(imagePath.string().c_str(), &width, &height,
                                  &nrChannels, STBI_rgb_alpha);
  if (!data) {
    throw std::runtime_error("Error - Texture not found at " +
                             imagePath.string());
  }

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Explicitly set unpack alignment to 1 for non-4-byte aligned rows
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Upload as GL_RGBA since STBI_rgb_alpha forces 4 channels
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  // Store in map AFTER generation succeeds
  texMap[name].texID = texID;
}

void TextureManager::makeTextures() {
  makeTexture("thumbsup", "textures/thumbsup.jpg");
  makeTexture("playerTank", "textures/tank2.png");
  makeTexture("bullet", "textures/bullet.png");
}
