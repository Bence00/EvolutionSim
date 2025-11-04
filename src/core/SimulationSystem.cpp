#include "core/SimulationSystem.hpp"
#include "core/Agent.hpp"           
#include <random>

SimulationSystem::SimulationSystem(WorldState& s)
    : state(s)
{
    reset();
    numAgentA = 100;
    numAgentB = 100;
}

void SimulationSystem::reset() {
    state.agents.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, state.gridWidth - 1);  
    std::uniform_int_distribution<> distY(0, state.gridHeight - 1);
  
    for (int i = 0; i < numAgentA; ++i) {
        sf::Vector2i cell{ distX(gen), distY(gen) };
        state.agents.push_back(std::make_unique<AgentA>(cell));
    }
    for (int i = 0; i < numAgentB; ++i) {
        sf::Vector2i cell{ distX(gen), distY(gen) };
        state.agents.push_back(std::make_unique<AgentB>(cell));
    }

}

void SimulationSystem::update(sf::Time dt) {
    if (!running) return;

    accum += dt.asSeconds() * simulation_speed;
    if (accum < 0.2f)
        return;
    accum = 0.f;

    movement();
}

void SimulationSystem::movement() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    for (auto& a : state.agents) {
        sf::Vector2i next = a->cell;

        int stepX = dist(gen) * a->speed;
        int stepY = dist(gen) * a->speed;

        next.x += stepX;
        next.y += stepY;

        if (next.x >= 0 && next.x < state.gridWidth &&
            next.y >= 0 && next.y < state.gridHeight)
        {
            a->cell = next;
            a->energy -= a->decay;
        }
    }
}
