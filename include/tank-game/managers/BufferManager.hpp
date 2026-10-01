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
  void bind();
};

class BufferManager {
 public:
  Buffer get(const std::string name);

  void makeBuffer(std::string name, GLenum target, GLenum usage);
  template <typename T>
  void updateBuffer(std::string name, const std::vector<T>& data) {
    auto& buf = bufferMap[name];
    buf.bind();
    glBufferData(buf.target, sizeof(T) * data.size(), data.data(), buf.usage);
  }

  void makeBuffers();

 private:
  std::map<std::string, Buffer> bufferMap;
};
}  // namespace Tnk
