#pragma once
#include <memory>

class tnk_Window;
class tnk_Renderer;
class tnk_ShaderManager;

class tnk_Engine {
 public:
  void startEngine();

  tnk_Engine();
  ~tnk_Engine();

 private:
  std::unique_ptr<tnk_Window> win;
  std::unique_ptr<tnk_Renderer> renderer;
  std::unique_ptr<tnk_ShaderManager> shaderMan;

  static void mainLoop(void* arg);
  void update();
};
