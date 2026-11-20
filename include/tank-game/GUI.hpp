#pragma once
namespace Tnk {
class Window;
struct GuiValues {
  float lightingResolution = 2.0;
  int gameResolution = 256;
  bool enableLighting = true;
  bool disableBots = false;
};
class Gui {
 public:
  Gui(Window& win) : win(win) {};
  void initGui();
  void shutdown();
  bool drawPauseMenu();
  bool drawGameOver(int score);
  void beginDraw();
  void endDraw();
  ~Gui();

  GuiValues values;

 private:
  Window& win;
};

}  // namespace Tnk
