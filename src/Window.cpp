#include "tank-game/Window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "emscripten/emscripten.h"
#include "glm/ext/vector_float2.hpp"
#include "tank-game/InputHelper.hpp"

using namespace Tnk;

EM_BOOL Window::OnCanvasResize(int eventType,
                               const EmscriptenUiEvent* e,
                               void* userData) {
  double cssWidth, cssHeight;
  emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

  double dpr = emscripten_get_device_pixel_ratio();
  int fbWidth = (int)(cssWidth * dpr);
  int fbHeight = (int)(cssHeight * dpr);

  emscripten_set_canvas_element_size("#canvas", fbWidth, fbHeight);

  glfwSetWindowSize(static_cast<Window*>(userData)->window, fbWidth, fbHeight);

  glViewport(0, 0, fbWidth, fbHeight);

  return EM_TRUE;
}

void Window::InitResizeHandling() {
  OnCanvasResize(0, nullptr, this);

  emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_FALSE,
                                 OnCanvasResize);
}
void Window::init() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init GLFW!");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(800, 600, "C++ WebGL 2 (GLFW)", nullptr, nullptr);
  InitResizeHandling();

  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Window not created!");
  }

  glfwMakeContextCurrent(window);

  glfwSetWindowUserPointer(window, &inputHelper);
  glfwSetKeyCallback(window, GLFWKeyCallback);
}

void Window::GLFWKeyCallback(GLFWwindow* win,
                             int key,
                             int scancode,
                             int action,
                             int mods) {
  InputHelper* inputHelper =
      static_cast<InputHelper*>(glfwGetWindowUserPointer(win));

  if (action == GLFW_PRESS) {
    inputHelper->setKeyState(key, true);
  }
  if (action == GLFW_RELEASE)
    inputHelper->setKeyState(key, false);
}

void Window::GLFWMouseCallback(GLFWwindow* window,
                               double xposIn,
                               double yposIn) {
  InputHelper* inputHelper =
      static_cast<InputHelper*>(glfwGetWindowUserPointer(window));

  inputHelper->mouseMoved(window, glm::vec2(xposIn, yposIn));
}
