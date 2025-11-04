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
    collision();
    movement(scaledDt);
}

void SimulationSystem::movement(float dt) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    const int W = state.gridWidth;
    const int H = state.gridHeight;

    for (std::size_t i = 0; i < state.agents.size(); ++i) {
        auto& a = state.agents[i];
        a->timeSinceMove += dt;

        if (a->timeSinceMove < a->speed)
            continue;

        a->timeSinceMove -= a->speed;

        sf::Vector2i current = a->cell;

        int stepX = dist(gen);
        int stepY = dist(gen);

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
