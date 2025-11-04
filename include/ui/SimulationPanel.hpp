#pragma once
#include <imgui.h>
#include "core/SimulationSystem.hpp"
#include "core/WorldState.hpp"

class SimulationPanel {
public:
    explicit SimulationPanel(SimulationSystem& sim, WorldState& world)
        : sim(sim), world(world) {}

    void draw() {
        ImGui::Begin("Simulation Settings");

        bool running = sim.isRunning();
        if (ImGui::Checkbox("Running", &running)) {
            sim.setRunning(running);
        }

        if (ImGui::Button("Reset")) {
            sim.reset();
        }

        int gridSize[2] = { world.gridWidth, world.gridHeight };
        if (ImGui::SliderInt2("Grid Size", gridSize, 10, 100)) {
            world.gridWidth  = gridSize[0];
            world.gridHeight = gridSize[1];
        }

        int aCount = sim.numAgentA;
        int bCount = sim.numAgentB;
        if (ImGui::SliderInt("Agent A count", &aCount, 0, 200)) {
            sim.numAgentA = aCount;
        }
        if (ImGui::SliderInt("Agent B count", &bCount, 0, 200)) {
            sim.numAgentB = bCount;
        }
        float sim_speed = sim.simulation_speed;
        if (ImGui::SliderFloat("Simulation Speed", &sim_speed, 0.1f, 50.0f)) {
            sim.simulation_speed = sim_speed;
        }

        ImGui::End();
    }

private:
    SimulationSystem& sim;
    WorldState& world;
};
