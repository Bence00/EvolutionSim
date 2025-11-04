#pragma once
#include <SFML/System/Time.hpp>
#include "core/WorldState.hpp"

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
    void movement(float dt);
    std::vector<int> occupancy;
    
    WorldState& state;

    bool  running = true;
    float accum = 0.f;
};
