#include "tank-game/GUI.hpp"
#include "external/imgui/backends/imgui_impl_glfw.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"
#include "external/imgui/imgui.h"
#include "tank-game/Window.hpp"
using namespace Tnk;

void Gui::initGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(win.window, true);
  ImGui_ImplOpenGL3_Init();
};

// Call this inside your main render loop when `isPaused` is true.
bool Gui::drawPauseMenu() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  static bool showSettings = false;
  bool gamePaused = true;

  const ImGuiViewport* viewport = ImGui::GetMainViewport();

  // 1. Dim the game screen behind the menu
  ImGui::GetBackgroundDrawList()->AddRectFilled(
      viewport->Pos,
      ImVec2(viewport->Pos.x + viewport->Size.x,
             viewport->Pos.y + viewport->Size.y),
      IM_COL32(0, 0, 0, 160)  // Semi-transparent black
  );

  // 2. Center the pause window on the screen
  ImVec2 center = viewport->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(320.0f, 0.0f),
                           ImGuiCond_Always);  // Auto-height

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

  // Styling tweaks for a cleaner game-menu look
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24.0f, 24.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 12.0f));

  if (ImGui::Begin("##PauseOverlay", nullptr, windowFlags)) {
    // Centered Title
    const char* title = showSettings ? "SETTINGS" : "GAME PAUSED";
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", title);

    ImGui::Separator();
    ImGui::Spacing();

    float buttonWidth = ImGui::GetContentRegionAvail().x;
    float buttonHeight = 38.0f;

    if (!showSettings) {
      if (ImGui::Button("Resume", ImVec2(buttonWidth, buttonHeight))) {
        gamePaused = false;
      }

      if (ImGui::Button("Settings", ImVec2(buttonWidth, buttonHeight))) {
        showSettings = true;
      }

      ImGui::Spacing();

    } else {
      // --- Settings Sub-Menu ---
      ImGui::Checkbox("Lobotomize bots", &values.disableBots);
      ImGui::Separator();
      ImGui::SliderInt("Game Resolution", &values.gameResolution, 100,
                       win.realSize.y);

      ImGui::Checkbox("Enable Lighting", &values.enableLighting);
      ImGui::SliderFloat("Lighting Resolution", &values.lightingResolution,
                         0.0f, 3.0f, "%.2f");

      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
        showSettings = false;
      }
    }
  }
  ImGui::End();
  ImGui::PopStyleVar(4);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return gamePaused;
}

Gui::~Gui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
