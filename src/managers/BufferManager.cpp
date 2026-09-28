#include "tank-game/managers/BufferManager.hpp"
#include <GLES3/gl3.h>
using namespace Tnk;
void BufferManager::makeBuffer(std::string name, GLenum target, GLenum usage) {
  bufferMap[name] = {.target = target, .usage = usage};
  glGenBuffers(1, &bufferMap[name].bufferID);
}
