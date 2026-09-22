#pragma once
namespace Tnk {

class Window;
class ShaderManager;

class Renderer {
 public:
  Renderer(Window& win, ShaderManager& shaderMan)
      : win(win), shaderMan(shaderMan) {}
  void drawScreen();

 private:
  Window& win;
  ShaderManager& shaderMan;
};
}  // namespace Tnk
