#include "ui/DebugPanel.hpp"

void DebugPanel::draw(sf::Time dt, const SimulationSystem& sim) {
    float fps = 1.f / dt.asSeconds();
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Simulation time: %.2f s", sim.elapsedSimTime);
    ImGui::End();
}
