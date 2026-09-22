#pragma once
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include "GLFW/glfw3.h"

namespace Tnk {

class Window {
 public:
  GLFWwindow* window = nullptr;
  void init();
  void InitResizeHandling();

 private:
  static EM_BOOL OnCanvasResize(int eventType,
                                const EmscriptenUiEvent* e,
                                void* userData);
};
}  // namespace Tnk
