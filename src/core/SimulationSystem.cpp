#include "core/SimulationSystem.hpp"
#include "core/Agent.hpp"
#include <random>
#include <algorithm>

SimulationSystem::SimulationSystem(WorldState& s)
    : state(s)
{
    numAgentA = 100;
    numAgentB = 100;
    reset();
}

void SimulationSystem::reset() {
    state.agents.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, state.gridWidth - 1);
    std::uniform_int_distribution<> distY(0, state.gridHeight - 1);
    std::uniform_real_distribution<float> speedDist(-0.25f, 0.25f);

    for (int i = 0; i < numAgentA; ++i) {
        sf::Vector2i cell{ distX(gen), distY(gen) };
        auto agent = std::make_unique<AgentA>(cell);

        agent->speed = std::max(0.1f, agent->speed + speedDist(gen));
        state.agents.push_back(std::move(agent));
    }

    for (int i = 0; i < numAgentB; ++i) {
        sf::Vector2i cell{ distX(gen), distY(gen) };
        auto agent = std::make_unique<AgentB>(cell);

        agent->speed = std::max(0.1f, agent->speed + speedDist(gen));
        state.agents.push_back(std::move(agent));
    }
}

void SimulationSystem::update(sf::Time dt) {
    if (!running) return;

    float scaledDt = dt.asSeconds() * simulation_speed;
    movement(scaledDt);
}

void SimulationSystem::movement(float dt) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    for (auto& a : state.agents) {
        a->timeSinceMove += dt;

        if (a->timeSinceMove < a->speed)
            continue;

        a->timeSinceMove -= a->speed;

        sf::Vector2i next = a->cell;

        int stepX = dist(gen);
        int stepY = dist(gen);

        next.x += stepX;
        next.y += stepY;

        if (next.x >= 0 && next.x < state.gridWidth &&
            next.y >= 0 && next.y < state.gridHeight)
        {
            a->cell = next;
        }
    }
}
