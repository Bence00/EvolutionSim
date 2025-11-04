#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Agent {
public:
    Agent(sf::Vector2i c, float spd, float dec)
        : cell(c), speed(spd), decay(dec), energy(100.f), size(0.4f) {}

    virtual ~Agent() = default;

    sf::Vector2i cell;
    float energy;
    float speed;
    float decay;
    float size;
    sf::Color color;
};

class AgentA : public Agent {
public:
    AgentA(sf::Vector2i c)
        : Agent(c, 1.0f, 1.0f)
    {
        color = sf::Color::Red; 
    }
};

class AgentB : public Agent {
public:
    AgentB(sf::Vector2i c)
        : Agent(c, 1.5f, 0.5f)
    {
        color = sf::Color::Blue; 
    }
};
