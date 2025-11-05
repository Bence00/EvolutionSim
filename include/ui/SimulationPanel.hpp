#pragma once
#include <imgui.h>
#include "core/SimulationSystem.hpp"
#include "core/WorldState.hpp"
#include "core/Agent.hpp"

class SimulationPanel {
public:
    explicit SimulationPanel(SimulationSystem& sim, WorldState& world)
        : sim(sim), world(world) {}

    bool draw() {
        bool resetPressed = false;

        ImGui::Begin("Simulation Settings");

        bool running = sim.isRunning();
        if (ImGui::Checkbox("Start/Pause", &running)) {
            sim.setRunning(running);
            // TODO: starting game previous config and new grid settings without crash
        }

        bool newborn = sim.isNewbornCooldownEnabled();   
        if (ImGui::Checkbox("Newborn Cooldown", &newborn)) { 
            sim.setNewbornCooldownEnabled(newborn);          
        }

        if (ImGui::Button("Reset")) {
            sim.reset();
            resetPressed = true;  
        }

        int gridSize[2] = { world.gridWidth, world.gridHeight };
        if (!running && ImGui::SliderInt2("Grid Size", gridSize, 10, 500)) {
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
        if (ImGui::SliderFloat("Simulation Speed", &sim_speed, 0.1f, 200.0f, "%.3fx", ImGuiSliderFlags_Logarithmic)) {
            sim.simulation_speed = sim_speed;
        }

        float cooldown = sim.getReproduceCooldown();
        if (ImGui::SliderFloat("Reproduce Cooldown", &cooldown, 0.1f, 200.f, "%.3fs", ImGuiSliderFlags_Logarithmic)) {
            sim.setReproduceCooldown(cooldown);
        }

        ImGui::SliderFloat("Default Lifetime", &sim.defaultLifetime, 10.f, 500.f, "%.3fs", ImGuiSliderFlags_Logarithmic);

        ImGui::End();

        return resetPressed;  
    }

private:
    SimulationSystem& sim;
    WorldState& world;
};
