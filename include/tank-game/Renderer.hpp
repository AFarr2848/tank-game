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
           TextureManager& texMan)
      : win(win), shaderMan(shaderMan), bufferMan(bufferMan), texMan(texMan) {}
  void init(GLuint cascadeProgram, GLuint averageProgram, GLuint mergeProgram);
  void drawScreen();

  void drawMaze(const std::vector<glm::vec2>& lines);

  void drawSprites(const std::vector<Sprite>& sprites);
  void drawImage();
  void drawCascades(std::array<PointLight, MAX_LIGHTS>& lights,
                    CascadeInfo cascadeInfo);
  void mergeCascades(CascadeInfo cascadeInfo);
  void combineLighting();

 private:
  Window& win;
  ShaderManager& shaderMan;
  BufferManager& bufferMan;
  TextureManager& texMan;
  CascadeInfoLocations cascadeLocations;
  CascadeInfoLocations mergeLocations;
  GLint average_screenSizeLocation;

  void setCascadeInfoUniforms(const CascadeInfoLocations& locs,
                              const CascadeInfo& info);
};
}  // namespace Tnk
