#pragma once
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "GLFW/glfw3.h"

class InputHelper;
namespace Tnk {

class Window {
 public:
  Window(InputHelper& inputHelper) : inputHelper(inputHelper) {};
  GLFWwindow* window = nullptr;
  void init();
  void InitResizeHandling();

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
