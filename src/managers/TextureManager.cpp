
#include "tank-game/managers/TextureManager.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLES3/gl3.h>
#include <stdexcept>
#include <vector>
#include "tank-game/gameObjects/Maze.hpp"
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

void TextureManager::makeTextureFromImage(
    std::string name,
    const std::filesystem::path imagePath) {
  int width, height, nrChannels;

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

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  texMap[name].texID = texID;
}

void TextureManager::makeBlankTexture(std::string name,
                                      int screenW,
                                      int screenH,
                                      GLint internalFormat,
                                      GLenum format,
                                      GLenum samplingMethod) {
  if (texMap.contains(name))
    glDeleteTextures(0, &texMap.at(name).texID);

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, screenW, screenH, 0, GL_RGBA,
               format, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

  texMap[name].texID = texID;
}

void TextureManager::makeTextures(int screenW, int screenH) {
  glGenFramebuffers(1, &fbo);
  makeTextureFromImage("thumbsup", "textures/thumbsup.jpg");
  makeTextureFromImage("playerTank", "textures/tank2.png");
  makeTextureFromImage("bullet", "textures/bullet.png");
  makeBufferTextures(screenW, screenH);
}
void TextureManager::makeBufferTextures(int screenW, int screenH) {
  makeBlankTexture("mazeTex", screenW, screenH, GL_RGBA8, GL_UNSIGNED_BYTE,
                   GL_NEAREST);
}

void TextureManager::makeLightingTextures(glm::vec2 texSize) {
  makeBlankTexture("cascade0", texSize.x, texSize.y, GL_RGBA16F, GL_HALF_FLOAT,
                   GL_NEAREST);
  makeBlankTexture("cascade1", texSize.x, texSize.y, GL_RGBA16F, GL_HALF_FLOAT,
                   GL_NEAREST);
  makeBlankTexture("cascade2", texSize.x, texSize.y, GL_RGBA16F, GL_HALF_FLOAT,
                   GL_NEAREST);
  makeBlankTexture("cascade3", texSize.x, texSize.y, GL_RGBA16F, GL_HALF_FLOAT,
                   GL_NEAREST);
  makeBlankTexture("lightingTex", texSize.x, texSize.y, GL_RGBA16F,
                   GL_HALF_FLOAT, GL_LINEAR);
  makeBlankTexture("tempLightingTex", texSize.x, texSize.y, GL_RGBA16F,
                   GL_HALF_FLOAT, GL_LINEAR);
  makeBlankTexture("mazeBakeTex", texSize.x, texSize.y, GL_RGBA16F,
                   GL_HALF_FLOAT, GL_NEAREST);
}

void TextureManager::makeWallTex(Maze& maze) {
  int N = maze.mazeSize;

  std::vector<uint8_t> data((N + 1) * (N + 1) * 2, 0);
  auto setV = [&](int i, int j) { data[(j * (N + 1) + i) * 2 + 0] = 255; };
  auto setH = [&](int i, int j) { data[(j * (N + 1) + i) * 2 + 1] = 255; };

  for (int r = 0; r < N; r++) {
    for (int c = 0; c < N; c++) {
      const Maze::MazeCell& cell = maze.cells[r][c];
      int i = c;
      int j = r;

      if (cell.wallWest)
        setV(i, j);
      if (cell.wallEast)
        setV(i + 1, j);
      if (cell.wallSouth)
        setH(i, j + 1);
      if (cell.wallNorth)
        setH(i, j);
    }
  }

  for (int k = 0; k < N; k++) {
    setV(0, k);
    setV(N, k);
    setH(k, 0);
    setH(k, N);
  }

  GLuint wallTex;
  glGenTextures(1, &wallTex);
  glBindTexture(GL_TEXTURE_2D, wallTex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, N + 1, N + 1, 0, GL_RG,
               GL_UNSIGNED_BYTE, data.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  texMap["wallTex"].texID = wallTex;
}
