#include "ui/DebugPanel.hpp"

void DebugPanel::draw(sf::Time dt) {
    float fps = 1.f / dt.asSeconds();
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::End();
}
