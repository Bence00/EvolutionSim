#include "core/SimulationSystem.hpp"
#include "core/Agent.hpp"
#include "globals.hpp" 
#include <algorithm>
#include <array>

namespace {
    static constexpr float MAX_GRID_OCCUPANCY = 0.7f;
    static constexpr int MAX_SPAWN_ATTEMPTS = 32;
    static constexpr float MIN_STEP_INTERVAL = 0.1f;
    static constexpr int INITIAL_NEWBORN_RESERVE = 100;
    static constexpr int LIFTIME_DISTRIBUTION_RANGE = 50;
    const sf::Color NEWBORN_HIGHLIGHT_COLOR = sf::Color::White;
    const sf::Color PARENT_HIGHLIGHT_COLOR = sf::Color::Yellow;
    const sf::Color AGENT_A_COLOR = sf::Color(106, 163, 217);
}

SimulationSystem::SimulationSystem(WorldState& s)
    : world1(s),
      gen(std::random_device{}()),
      speedDist(-0.25f, 0.25f)
{
    numAgentA = 50;
    numAgentB = 50;
    newbornCooldownEnabled = true; 
    reset();
}

void SimulationSystem::reset() {
    world1.agents.clear();

    int W = std::max(1, world1.gridWidth);
    int H = std::max(1, world1.gridHeight);
    int cells = W * H;

    distX = std::uniform_int_distribution<>(0, W - 1);
    distY = std::uniform_int_distribution<>(0, H - 1);

    occupancy.assign(cells, -1);

    for (int i = 0; i < numAgentA; ++i)
        spawnAgentA();

    for (int i = 0; i < numAgentB; ++i)
        spawnAgentB();

    elapsedSimTime = 0.f;
}

void SimulationSystem::update(sf::Time dt) {
    if (!running) return;
    float scaledDt = dt.asSeconds() * simulation_speed;
    elapsedSimTime += scaledDt;
    movement(scaledDt);

    for (auto& a : world1.agents) {
        if (a->reproduceCooldown > 0.f)
            a->reproduceCooldown -= dt.asSeconds();
    }

    const float deathRate = simulation_speed;  
    for (auto& a : world1.agents) {
        a->timeUntilDeath(dt.asSeconds(), deathRate);
    }

    const int W = world1.gridWidth;
    std::size_t i = 0;
    while (i < world1.agents.size()) {
        auto& agent = world1.agents[i];
        
        if (agent->lifeTime <= 0.f) {
            const int deadIdx = agent->cell.y * W + agent->cell.x;
            if (deadIdx >= 0 && deadIdx < occupancy.size() && occupancy[deadIdx] == static_cast<int>(i)) {
                occupancy[deadIdx] = -1;
            }

            std::size_t lastIdx = world1.agents.size() - 1;

            if (i != lastIdx) {
                world1.agents[i] = std::move(world1.agents[lastIdx]);
                
                const int movedIdx = world1.agents[i]->cell.y * W + world1.agents[i]->cell.x;

                if (movedIdx >= 0 && movedIdx < occupancy.size() && occupancy[movedIdx] == static_cast<int>(lastIdx)) {
                     occupancy[movedIdx] = static_cast<int>(i);
                }
            }

            world1.agents.pop_back();

        }
        else {
            ++i;
        }
    }
    if (world1.agents.empty()) {
        running = false;     
        return;              
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

        if (a->timeSinceMove > a->stepInterval * 2.f)
            a->timeSinceMove = a->stepInterval;

        if (a->timeSinceMove < a->stepInterval)
            continue;

        a->timeSinceMove -= a->stepInterval;

        sf::Vector2i current = a->cell;

        if (current.x < 0 || current.x >= W || current.y < 0 || current.y >= H) {
            continue;
        }

        int stepX = stepDist(gen);
        int stepY = stepDist(gen);

        sf::Vector2i next = current;
        next.x += stepX;
        next.y += stepY;

        if (next.x < 0 || next.x >= W || next.y < 0 || next.y >= H)
            continue;

        int nextIdx = next.y * W + next.x;
        int curIdx  = current.y * W + current.x;

        if (occupancy[nextIdx] == -1)
        {
            occupancy[curIdx] = -1;
            occupancy[nextIdx] = static_cast<int>(i); 
            a->cell = next;
        }
    }
}

void SimulationSystem::repopulate() {
    const int W = world1.gridWidth;
    const int H = world1.gridHeight;
    const int maxAgents = static_cast<int>(W * H * MAX_GRID_OCCUPANCY);

    if (static_cast<int>(world1.agents.size()) >= maxAgents)
        return;

    const std::array<sf::Vector2i, 4> directions = {
        sf::Vector2i{ 1, 0 },
        sf::Vector2i{-1, 0 },
        sf::Vector2i{ 0, 1 },
        sf::Vector2i{ 0,-1 }
    };

    std::vector<std::unique_ptr<Agent>> newborns;
    newborns.reserve(INITIAL_NEWBORN_RESERVE);

    std::uniform_int_distribution<> typeDist(0, 1);

    auto trySpawnAround = [&](const sf::Vector2i& base) -> bool {
        if (static_cast<int>(world1.agents.size()) + static_cast<int>(newborns.size()) >= maxAgents)
            return false;

        for (const auto& d : directions) {
            sf::Vector2i pos = base + d;
            if (pos.x < 0 || pos.x >= W || pos.y < 0 || pos.y >= H)
                continue;

            int idx = pos.y * W + pos.x;
            if (occupancy[idx] == -1) {
                std::unique_ptr<Agent> newAgent;

                AgentType type = (typeDist(gen) == 0) ? AgentType::A : AgentType::B;

                if (type == AgentType::A)
                    newAgent = std::make_unique<AgentA>(pos);
                else
                    newAgent = std::make_unique<AgentB>(pos);

                if (newbornCooldownEnabled) {
                    newAgent->reproduceCooldown = reproduceCooldownSec;
                }
                int random_lifetime = std::uniform_int_distribution<>(-LIFTIME_DISTRIBUTION_RANGE,LIFTIME_DISTRIBUTION_RANGE)(gen);
                newAgent->lifeTime = defaultLifetime + random_lifetime;

                if(newbornCooldownEnabled){
                    highlightAgent(*newAgent, NEWBORN_HIGHLIGHT_COLOR,  reproduceCooldownSec);
                }

                int newbornIndex = static_cast<int>(world1.agents.size() + newborns.size());
                occupancy[idx] = newbornIndex;
                
                newborns.push_back(std::move(newAgent));
                return true;
            }
        }
        return false;
    };

    const std::size_t currentAgentCount = world1.agents.size();
    for (std::size_t i = 0; i < currentAgentCount; ++i) {
        auto& a = world1.agents[i];

        if (a->reproduceCooldown > 0.f)
            continue;

        bool aIsA = (a->type == AgentType::A);
        bool aIsB = (a->type == AgentType::B);

        if (!aIsA && !aIsB) 
             continue;

        for (const auto& d : directions) {
            if (static_cast<int>(world1.agents.size()) + static_cast<int>(newborns.size()) >= maxAgents)
                break;

            sf::Vector2i npos = a->cell + d;
            if (npos.x < 0 || npos.x >= W || npos.y < 0 || npos.y >= H)
                continue;

            int nIdx = occupancy[npos.y * W + npos.x];
            if (nIdx == -1)
                continue;

            if (nIdx < 0 || nIdx >= static_cast<int>(world1.agents.size()))
                continue; 

            auto& b = world1.agents[nIdx];

            bool bIsA = (b->type == AgentType::A);
            bool bIsB = (b->type == AgentType::B);

            bool opposite = (aIsA && bIsB) || (aIsB && bIsA);
            if (!opposite)
                continue;

            bool spawned = trySpawnAround(a->cell);
            if (!spawned)
                spawned = trySpawnAround(b->cell);

            if (spawned) {
                a->reproduceCooldown = reproduceCooldownSec;
                b->reproduceCooldown = reproduceCooldownSec;

                highlightAgent(*a, PARENT_HIGHLIGHT_COLOR, reproduceCooldownSec);
                highlightAgent(*b, PARENT_HIGHLIGHT_COLOR, reproduceCooldownSec);
                break; 
            }
        }
    }

    for (auto& n : newborns) {
        if (static_cast<int>(world1.agents.size()) >= maxAgents)
            break;
        world1.agents.push_back(std::move(n));
    }
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
void SimulationSystem::spawnAgentA() {
    const int W = world1.gridWidth;
    const int H = world1.gridHeight;
    const int cells = W * H;
    const int maxAgents = static_cast<int>(cells * MAX_GRID_OCCUPANCY);

    if ((int)world1.agents.size() >= maxAgents)
        return;

    if ((int)occupancy.size() != cells) {
        collision(); 
    }

    sf::Vector2i cell{-1, -1};
    bool found = false;

    for (int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; ++attempt) {
        int x = distX(gen);
        int y = distY(gen);
        int idx = y * W + x;

        if (idx >= 0 && idx < (int)occupancy.size() && occupancy[idx] == -1) {
            cell = {x, y};
            found = true;
            break;
        }
    }

    if (!found)
        return;

    auto agent = std::make_unique<AgentA>(cell);
    agent->stepInterval = std::max(MIN_STEP_INTERVAL, agent->stepInterval + speedDist(gen));
    agent->color = AGENT_A_COLOR;

    int random_lifetime = std::uniform_int_distribution<>(-LIFTIME_DISTRIBUTION_RANGE,LIFTIME_DISTRIBUTION_RANGE)(gen);
    agent->lifeTime = defaultLifetime + random_lifetime;

    int idx = cell.y * W + cell.x;
    occupancy[idx] = (int)world1.agents.size(); 

    world1.agents.push_back(std::move(agent));
}
void SimulationSystem::spawnAgentB() {
    const int W = world1.gridWidth;
    const int H = world1.gridHeight;
    const int cells = W * H;
    const int maxAgents = static_cast<int>(cells * MAX_GRID_OCCUPANCY);

    if ((int)world1.agents.size() >= maxAgents)
        return;

    if ((int)occupancy.size() != cells) {
        collision(); 
    }

    sf::Vector2i cell{-1, -1};
    bool found = false;

    for (int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; ++attempt) {
        int x = distX(gen);
        int y = distY(gen);
        int idx = y * W + x;

        if (idx >= 0 && idx < (int)occupancy.size() && occupancy[idx] == -1) {
            cell = {x, y};
            found = true;
            break;
        }
    }

    if (!found)
        return;

    auto agent = std::make_unique<AgentB>(cell);
    agent->stepInterval = std::max(MIN_STEP_INTERVAL, agent->stepInterval + speedDist(gen));

    int random_lifetime = std::uniform_int_distribution<>(-LIFTIME_DISTRIBUTION_RANGE,LIFTIME_DISTRIBUTION_RANGE)(gen);
     agent->lifeTime = defaultLifetime + random_lifetime;

    int idx = cell.y * W + cell.x;
    occupancy[idx] = (int)world1.agents.size();

    world1.agents.push_back(std::move(agent));
}
void SimulationSystem::highlightAgent(Agent& agent, sf::Color newColor, float duration) {
    if (agent.colorTimer <= 0.f) {
        agent.originalColor = agent.color;
    }
    agent.color = newColor;
    agent.colorTimer = duration;
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
void SimulationSystem::agentDeath(std::size_t index)
{
    if (index >= world1.agents.size())
        return;

    world1.agents.erase(world1.agents.begin() + static_cast<long>(index));
}