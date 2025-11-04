#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Agent {
public:
    Agent(sf::Vector2i c, float spd, sf::Color col = sf::Color::Red)
        : cell(c),
          energy(100.f),
          speed(spd),
          size(0.4f),
          color(col),
          timeSinceMove(0.f)
    {}

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
    AgentA(sf::Vector2i c, float spd = 1.0f)
        : Agent(c, spd, sf::Color(106, 163, 217))
    {}
};

class AgentB: public Agent {
public:
    AgentB(sf::Vector2i c, float spd = 1.0f)
        : Agent(c, spd, sf::Color(198, 118, 232))
    {}
};
