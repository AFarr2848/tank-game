#pragma once
#include <memory>

namespace Tnk {

class Window;
class Renderer;
class ShaderManager;

class Engine {
 public:
  void startEngine();

  Engine();
  ~Engine();

 private:
  std::unique_ptr<Window> win;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<ShaderManager> shaderMan;

  static void mainLoop(void* arg);
  void update();
};
}  // namespace Tnk
