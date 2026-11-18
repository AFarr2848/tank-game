#pragma once
namespace Tnk {
class Window;
struct GuiValues {
  float lightingResolution = 1.0;
  int gameResolution = 320;
  bool enableLighting = true;
  bool disableBots = false;
};
class Gui {
 public:
  Gui(Window& win) : win(win) {};
  void initGui();
  void shutdown();
  bool drawPauseMenu();
  ~Gui();

  GuiValues values;

 private:
  Window& win;
};

}  // namespace Tnk
