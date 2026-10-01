#include "tank-game/managers/BufferManager.hpp"
#include <GLES3/gl3.h>
#include <stdexcept>
#include "glm/ext/vector_float2.hpp"
using namespace Tnk;
void BufferManager::makeBuffer(std::string name, GLenum target, GLenum usage) {
  bufferMap[name] = {.target = target, .usage = usage};
  glGenBuffers(1, &bufferMap[name].bufferID);
}

void BufferManager::makeBuffers() {
  std::vector<glm::vec2> squareVertices = {{1, 1},  {1, -1}, {-1, -1},
                                           {-1, 1}, {1, 1},  {-1, -1}};

  makeBuffer("lineBuffer", GL_ARRAY_BUFFER, GL_STATIC_DRAW);
  makeBuffer("spriteBuffer", GL_ARRAY_BUFFER, GL_STATIC_DRAW);
  updateBuffer("spriteBuffer", squareVertices);
}

Buffer BufferManager::get(const std::string name) {
  try {
    return bufferMap.at(name);
  } catch (std::out_of_range& e) {
    throw std::runtime_error("Buffer '" + name + "' not found!");
  }
}

void Buffer::bind() {
  glBindBuffer(this->target, this->bufferID);
}
