#pragma once
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "GLFW/glfw3.h"
#include "glm/ext/vector_int2.hpp"

namespace Tnk {
class InputHelper;

class Window {
 public:
  Window(InputHelper& inputHelper) : inputHelper(inputHelper) {};
  GLFWwindow* window = nullptr;
  void init();
  void InitResizeHandling();
  glm::ivec2 renderSize;
  glm::ivec2 realSize;
  bool resized = false;

 private:
  InputHelper& inputHelper;
  static EM_BOOL OnCanvasResize(int eventType,
                                const EmscriptenUiEvent* e,
                                void* userData);

  static void GLFWMouseCallback(GLFWwindow*, double, double);
  static void GLFWKeyCallback(GLFWwindow* win,
                              int key,
                              int scancode,
                              int action,
                              int mods);
};
}  // namespace Tnk
