
#pragma once
class tnk_Window;
class tnk_ShaderManager;

class tnk_Renderer {
 public:
  tnk_Renderer(tnk_Window& win, tnk_ShaderManager& shaderMan)
      : win(win), shaderMan(shaderMan) {}
  void drawScreen();

 private:
  tnk_Window& win;
  tnk_ShaderManager& shaderMan;
};
