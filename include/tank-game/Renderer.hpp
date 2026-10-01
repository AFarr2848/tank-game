#pragma once
#include <vector>
#include "glm/ext/vector_float2.hpp"
#include "tank-game/gameObjects/Sprite.hpp"
namespace Tnk {

class Window;
class ShaderManager;
class BufferManager;

class Renderer {
 public:
  Renderer(Window& win, ShaderManager& shaderMan, BufferManager& bufferMan)
      : win(win), shaderMan(shaderMan), bufferMan(bufferMan) {}
  void drawScreen();

  void drawMaze(const std::vector<glm::vec2>& lines);

  void drawSprites(const std::vector<Sprite>& sprites);

 private:
  Window& win;
  ShaderManager& shaderMan;
  BufferManager& bufferMan;
};
}  // namespace Tnk
