#pragma once
#include <vector>
#include "glm/ext/vector_float2.hpp"
#include "tank-game/Lighting.hpp"
#include "tank-game/gameObjects/Sprite.hpp"
namespace Tnk {

class Window;
class ShaderManager;
class BufferManager;
class TextureManager;

class Renderer {
 public:
  Renderer(Window& win,
           ShaderManager& shaderMan,
           BufferManager& bufferMan,
           TextureManager& texMan,
           Lighting& lighting)
      : win(win),
        shaderMan(shaderMan),
        bufferMan(bufferMan),
        texMan(texMan),
        lighting(lighting) {}
  void drawScreen();

  void drawMaze(const std::vector<glm::vec2>& lines);

  void drawSprites(const std::vector<Sprite>& sprites);
  void drawImage();
  void drawCascades(std::array<PointLight, MAX_LIGHTS>& lights);
  void mergeCascades();
  void combineLighting();

 private:
  Window& win;
  ShaderManager& shaderMan;
  BufferManager& bufferMan;
  TextureManager& texMan;
  Lighting& lighting;
};
}  // namespace Tnk
