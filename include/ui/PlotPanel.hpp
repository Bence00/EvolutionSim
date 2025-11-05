#pragma once
#include <imgui.h>
#include <implot.h>
#include <SFML/System/Time.hpp>
#include "core/WorldState.hpp"
#include "core/SimulationSystem.hpp"
#include <algorithm>

class PlotPanel {
public:
    explicit PlotPanel(SimulationSystem& sim, WorldState& world)
        : sim(sim), world(world)
    {
        agentHistory.reserve(MAX_SAMPLES);
        timeHistory.reserve(MAX_SAMPLES);
    }

    void update(sf::Time) {
        float agents = static_cast<float>(world.agents.size());
        float t = sim.elapsedSimTime; 

        if (agentHistory.size() >= MAX_SAMPLES) {
            shiftLeft(agentHistory);
            shiftLeft(timeHistory);
        }

        agentHistory.push_back(agents);
        timeHistory.push_back(t);
    }

    void draw() {
    ImGui::Begin("Simulation Graphs");
    ImVec2 size = ImGui::GetContentRegionAvail();

    if (!agentHistory.empty()) {
        float minX = timeHistory.front();
        float maxX = timeHistory.back();

        const int W = world.gridWidth;
        const int H = world.gridHeight;
        const float MAX_GRID_OCCUPANCY = 0.75f;
        const float maxAgents = static_cast<float>(W * H * MAX_GRID_OCCUPANCY);

        float xMargin = (maxX - minX) * 0.05f;

        if (ImPlot::BeginPlot("Agent Population", size)) {
            ImPlot::SetupAxes("Time (s)", "Agent count",
                            ImPlotAxisFlags_None, ImPlotAxisFlags_None);

            ImPlot::SetupAxesLimits(minX, maxX + xMargin, 0.0f, maxAgents, ImPlotCond_Always);

            ImPlot::PlotLine("Agents",
                            timeHistory.data(),
                            agentHistory.data(),
                            static_cast<int>(agentHistory.size()));

            ImPlot::EndPlot();
        }
    }

    ImGui::End();
}


    void reset() {
        agentHistory.clear();
        timeHistory.clear();
    }

private:
    SimulationSystem& sim;
    WorldState& world;

    std::vector<float> agentHistory;
    std::vector<float> timeHistory;

    static constexpr std::size_t MAX_SAMPLES = 2000;

    static void shiftLeft(std::vector<float>& v) {
        if (v.empty()) return;
        for (size_t i = 1; i < v.size(); ++i)
            v[i - 1] = v[i];
        v.pop_back();
    }
};
