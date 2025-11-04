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
    void collision();

    int numAgentA = 100;
    int numAgentB = 100;
    float simulation_speed = 1.0f;

private:
    WorldState& state;

    // RNG-k
    std::mt19937 gen;
    std::uniform_int_distribution<> distX;
    std::uniform_int_distribution<> distY;
    std::uniform_real_distribution<float> speedDist;

    std::vector<int> occupancy;

    bool  running = true;

    void movement(float dt);
    sf::Vector2i randomCell();
    void spawnPredator();
    void spawnAgentA();
    void spawnAgentB();
};
