#pragma once
#include <SFML/System/Time.hpp>
#include "core/WorldState.hpp"
#include <random>
#include <vector>

class SimulationSystem {
public:
    explicit SimulationSystem(WorldState& s);

    void reset();
    void update(sf::Time dt);

    void setRunning(bool r) { running = r; }
    bool isRunning() const { return running; }

    void setSpeed(float s) { simulation_speed = s; }

    void setReproduceCooldown(float v) { reproduceCooldownSec = v; }
    float getReproduceCooldown() const { return reproduceCooldownSec; }

    void collision();
    void repopulate();

    void setNewbornCooldownEnabled(bool enabled) { newbornCooldownEnabled = enabled; }
    bool isNewbornCooldownEnabled() const { return newbornCooldownEnabled; }

    int numAgentA = 2;
    int numAgentB = 2;
    float simulation_speed = 100.0f;
    float defaultLifetime = 500.f; 
    float elapsedSimTime = 0.f; 

private:
    WorldState& world1;

    std::mt19937 gen;
    std::uniform_int_distribution<> distX;
    std::uniform_int_distribution<> distY;
    std::uniform_real_distribution<float> speedDist;

    std::vector<int> occupancy;

    std::uint64_t spawnCounterA = 0;
    std::uint64_t spawnCounterB = 0;

    float reproduceCooldownSec = Agent::defaultReproduceCooldown;
    bool  running = false;

    bool newbornCooldownEnabled = true; 

    void movement(float dt);
    void updateColors(float dt);
    void highlightAgent (Agent& agent, sf::Color newColor, float duration);
    
    void agentDeath(std::size_t index);

    void spawnPredator();
    void spawnAgentA();
    void spawnAgentB();
};
