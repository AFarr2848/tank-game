#pragma once
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <map>
#include <string>
#include <vector>
namespace Tnk {

class Buffer {
 public:
  GLuint bufferID;
  GLenum target;
  GLenum usage;
};

class BufferManager {
 public:
  std::map<std::string, Buffer> bufferMap;
  void makeBuffer(std::string name, GLenum target, GLenum usage);
  template <typename T>
  void updateBuffer(std::string name, const std::vector<T>& data) {
    auto& buf = bufferMap[name];
    glBindBuffer(buf.target, buf.bufferID);
    glBufferData(buf.target, sizeof(T) * data.size(), data.data(), buf.usage);
  }

 private:
};
}  // namespace Tnk
