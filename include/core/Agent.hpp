#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Agent {
public:
    Agent(sf::Vector2i c, float spd)
        : cell(c), speed(spd), energy(100.f), size(0.4f), timeSinceMove(0.f) {}

    virtual ~Agent() = default;

    sf::Vector2i cell;
    float energy;
    float speed;
    float size;
    sf::Color color;

     float timeSinceMove; 
};

class AgentA : public Agent {
public:
    AgentA(sf::Vector2i c)
        : Agent(c, 1.0f)
    {
        color = sf::Color(106, 163, 217);
    }
};

class AgentB : public Agent {
public:
    AgentB(sf::Vector2i c)
        : Agent(c, 1.0f)
    {
        color = sf::Color(198, 118, 232); 
    }
};
