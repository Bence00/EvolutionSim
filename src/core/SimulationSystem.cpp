#include "core/SimulationSystem.hpp"
#include "core/Agent.hpp"
#include <algorithm>

SimulationSystem::SimulationSystem(WorldState& s)
    : state(s),
      gen(std::random_device{}()),
      distX(0, 0),
      distY(0, 0),
      speedDist(-0.25f, 0.25f)
{
    numAgentA = 100;
    numAgentB = 100;
    reset();
}

void SimulationSystem::reset() {
    state.agents.clear();

    int W = std::max(1, state.gridWidth);
    int H = std::max(1, state.gridHeight);

    distX = std::uniform_int_distribution<>(0, W - 1);
    distY = std::uniform_int_distribution<>(0, H - 1);

    spawnPredator();

    for (int i = 0; i < numAgentA; ++i)
        spawnAgentA();

    for (int i = 0; i < numAgentB; ++i)
        spawnAgentB();
}

void SimulationSystem::update(sf::Time dt) {
    if (!running) return;

    float scaledDt = dt.asSeconds() * simulation_speed;
    collision();
    movement(scaledDt);
}

void SimulationSystem::movement(float dt) {
    std::uniform_int_distribution<> stepDist(-1, 1);

    const int W = state.gridWidth;
    const int H = state.gridHeight;

    for (std::size_t i = 0; i < state.agents.size(); ++i) {
        auto& a = state.agents[i];
        a->timeSinceMove += dt;

        if (a->timeSinceMove < a->speed)
            continue;

        a->timeSinceMove -= a->speed;

        sf::Vector2i current = a->cell;

        int stepX = stepDist(gen);
        int stepY = stepDist(gen);

        sf::Vector2i next = current;
        next.x += stepX;
        next.y += stepY;

        if (next.x < 0 || next.x >= W || next.y < 0 || next.y >= H)
            continue;

        int nextIdx = next.y * W + next.x;
        int curIdx  = current.y * W + current.x;

        if (occupancy[nextIdx] == -1) {
            occupancy[curIdx] = -1;
            occupancy[nextIdx] = static_cast<int>(i);
            a->cell = next;
        }
    }
}

void SimulationSystem::collision() {
    const int W = state.gridWidth;
    const int H = state.gridHeight;
    const int cells = W * H;

    if ((int)occupancy.size() != cells) {
        occupancy.assign(cells, -1);
    } else {
        std::fill(occupancy.begin(), occupancy.end(), -1);
    }

    for (std::size_t i = 0; i < state.agents.size(); ++i) {
        const auto& a = state.agents[i];
        const int x = a->cell.x;
        const int y = a->cell.y;

        if (x >= 0 && x < W && y >= 0 && y < H) {
            occupancy[y * W + x] = static_cast<int>(i);
        }
    }
}

void SimulationSystem::spawnPredator() {
    auto cell = randomCell();
    auto predator = std::make_unique<Agent>(cell, 0.1f);
    predator->color = sf::Color::Red;
    state.agents.push_back(std::move(predator));
}

void SimulationSystem::spawnAgentA() {
    auto cell = randomCell();
    auto agent = std::make_unique<AgentA>(cell);
    agent->speed = std::max(0.1f, agent->speed + speedDist(gen));
    state.agents.push_back(std::move(agent));
}

void SimulationSystem::spawnAgentB() {
    auto cell = randomCell();
    auto agent = std::make_unique<AgentB>(cell);
    agent->speed = std::max(0.1f, agent->speed + speedDist(gen));
    state.agents.push_back(std::move(agent));
}

sf::Vector2i SimulationSystem::randomCell() {
    int x = distX(gen);
    int y = distY(gen);
    return { x, y };
}
