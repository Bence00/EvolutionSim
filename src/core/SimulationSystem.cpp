#include "core/SimulationSystem.hpp"
#include "core/Agent.hpp"
#include <algorithm>
#include "globals.hpp"
#include <array>

SimulationSystem::SimulationSystem(WorldState& s)
    : world1(s),
      gen(std::random_device{}()),
      distX(0, 0),
      distY(0, 0),
      speedDist(-0.25f, 0.25f)
{
    numAgentA = 4;
    numAgentB = 4;
    reset();
}

void SimulationSystem::reset() {
    world1.agents.clear();

    int W = std::max(1, world1.gridWidth);
    int H = std::max(1, world1.gridHeight);

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

    for (auto& a : world1.agents) {
    if (a->reproduceCooldown > 0.f)
        a->reproduceCooldown -= dt.asSeconds();
    }
    repopulate(); 
    updateColors(scaledDt);

}

void SimulationSystem::movement(float dt) {
    std::uniform_int_distribution<> stepDist(-1, 1);

    const int W = world1.gridWidth;
    const int H = world1.gridHeight;

    for (std::size_t i = 0; i < world1.agents.size(); ++i) {
        auto& a = world1.agents[i];
        a->timeSinceMove += dt;

        if (a->timeSinceMove < a->stepInterval)
            continue;

        a->timeSinceMove -= a->stepInterval;

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

void SimulationSystem::repopulate() {
    const int W = world1.gridWidth;
    const int H = world1.gridHeight;

    // 4 irány
    const std::array<sf::Vector2i, 4> directions = {
        sf::Vector2i{ 1, 0 },
        sf::Vector2i{-1, 0 },
        sf::Vector2i{ 0, 1 },
        sf::Vector2i{ 0,-1 }
    };

    std::vector<std::unique_ptr<Agent>> newborns;

    std::uniform_int_distribution<> typeDist(0, 1); // 0 = A, 1 = B

    auto trySpawnAround = [&](const sf::Vector2i& base) -> bool {
        for (const auto& d : directions) {
            sf::Vector2i pos = base + d;
            if (pos.x < 0 || pos.x >= W || pos.y < 0 || pos.y >= H)
                continue;

            int idx = pos.y * W + pos.x;
            if (occupancy[idx] == -1) {
                int type = typeDist(gen);  // véletlen döntés

                if (type == 0)
                    newborns.push_back(std::make_unique<AgentA>(pos));
                else
                    newborns.push_back(std::make_unique<AgentB>(pos));

                return true;
            }
        }
        return false;
    };

    for (std::size_t i = 0; i < world1.agents.size(); ++i) {
        auto& a = world1.agents[i];

        if (a->reproduceCooldown > 0.f)
            continue;

        bool aIsA = dynamic_cast<AgentA*>(a.get()) != nullptr;
        bool aIsB = dynamic_cast<AgentB*>(a.get()) != nullptr;
        if (!aIsA && !aIsB)
            continue;

        for (const auto& d : directions) {
            sf::Vector2i npos = a->cell + d;
            if (npos.x < 0 || npos.x >= W || npos.y < 0 || npos.y >= H)
                continue;

            int nIdx = occupancy[npos.y * W + npos.x];
            if (nIdx == -1)
                continue;

            auto& b = world1.agents[nIdx];

            bool bIsA = dynamic_cast<AgentA*>(b.get()) != nullptr;
            bool bIsB = dynamic_cast<AgentB*>(b.get()) != nullptr;

            bool opposite = (aIsA && bIsB) || (aIsB && bIsA);
            if (!opposite)
                continue;

            bool spawned = false;
            spawned = trySpawnAround(a->cell);
            if (!spawned)
                spawned = trySpawnAround(b->cell);

            if (spawned) {
                b->reproduceCooldown = Agent::defaultReproduceCooldown;

                highlightAgent(*a, sf::Color::Yellow, Agent::defaultReproduceCooldown);
                highlightAgent(*b, sf::Color::Yellow, Agent::defaultReproduceCooldown);
            }
        }
    }

    for (auto& n : newborns)
        world1.agents.push_back(std::move(n));

    collision();
}

void SimulationSystem::collision() {
    const int W = world1.gridWidth;
    const int H = world1.gridHeight;
    const int cells = W * H;

    if ((int)occupancy.size() != cells) {
        occupancy.assign(cells, -1);
    } else {
        std::fill(occupancy.begin(), occupancy.end(), -1);
    }

    for (std::size_t i = 0; i < world1.agents.size(); ++i) {
        const auto& a = world1.agents[i];
        const int x = a->cell.x;
        const int y = a->cell.y;

        if (x >= 0 && x < W && y >= 0 && y < H) {
            occupancy[y * W + x] = static_cast<int>(i);
        }
    }
}

void SimulationSystem::spawnPredator() {
    auto cell = randomCell();
    auto predator = std::make_unique<Agent>(cell, 1.0f);
    predator->color = sf::Color::Red;
    world1.agents.push_back(std::move(predator));
}

void SimulationSystem::spawnAgentA() {
    auto cell = randomCell();
    auto agent = std::make_unique<AgentA>(cell);
    agent->stepInterval = std::max(0.1f, agent->stepInterval + speedDist(gen));
    agent->color = sf::Color(106, 163, 217);
    world1.agents.push_back(std::move(agent));
}

void SimulationSystem::spawnAgentB() {
    auto cell = randomCell();
    auto agent = std::make_unique<AgentB>(cell);
    agent->stepInterval = std::max(0.1f, agent->stepInterval + speedDist(gen));
    world1.agents.push_back(std::move(agent));
}

sf::Vector2i SimulationSystem::randomCell() {
    int x = distX(gen);
    int y = distY(gen);
    return { x, y };
}
void SimulationSystem::updateColors(float dt) {
    for (auto& a : world1.agents) {
        if (a->colorTimer > 0.f) {
            a->colorTimer -= dt;
            if (a->colorTimer <= 0.f) {
                a->color = a->originalColor;
                a->colorTimer = 0.f;
            }
        }
    }
}
void SimulationSystem::highlightAgent(Agent& agent, sf::Color newColor, float duration) {
    agent.originalColor = agent.color;
    agent.color = newColor;
    agent.colorTimer = duration;
}
