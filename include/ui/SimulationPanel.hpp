#pragma once
#include <imgui.h>
#include "core/SimulationSystem.hpp"
#include "core/WorldState.hpp"
#include "core/Agent.hpp"

class SimulationPanel {
public:
    explicit SimulationPanel(SimulationSystem& sim, WorldState& world)
        : sim(sim), world(world) {}

    void draw() {
        ImGui::Begin("Simulation Settings");

        bool running = sim.isRunning();
        if (ImGui::Checkbox("Start/Pause", &running)) {
            sim.setRunning(running);
        }

        if (ImGui::Button("Reset")) {
            sim.reset();
        }

        int gridSize[2] = { world.gridWidth, world.gridHeight };
        if (!running && ImGui::SliderInt2("Grid Size", gridSize, 10, 200)) {
            world.gridWidth  = gridSize[0];
            world.gridHeight = gridSize[1];
            //sim.reset();
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
        if (ImGui::SliderFloat("Simulation Speed", &sim_speed, 0.1f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
            sim.simulation_speed = sim_speed;
        }

        static float cooldown = Agent::defaultReproduceCooldown;  
        if (ImGui::SliderFloat("Reproduce cooldown", &cooldown, 0.5f, 200.0f, "%.1f s")) {
            for (auto& ag : world.agents) {
                ag->reproduceCooldown = cooldown;
            }
        }

        ImGui::End();
    }

private:
    SimulationSystem& sim;
    WorldState& world;
};
