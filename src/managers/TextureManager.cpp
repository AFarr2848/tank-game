
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
  Texture& tex = texMap[name];
  glGenTextures(1, &tex.texID);
  glBindTexture(GL_TEXTURE_2D, tex.texID);

  // should prolly add config for this stuff at some point
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char* data =
      stbi_load(imagePath.string().c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("Error - Texture not found at " +
                             imagePath.string());
  }
  stbi_image_free(data);
}

void TextureManager::makeTextures() {
  makeTexture("thumbsup", "textures/thumbsup.jpg");
}
