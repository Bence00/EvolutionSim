#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Agent {
public:
    Agent(sf::Vector2i c, float spd, sf::Color col = sf::Color::Red)
        : cell(c),
          lifeTime(0.0f),
          stepInterval(spd),
          size(0.3f),
          color(col),
          originalColor(col),
          timeSinceMove(0.f),
          reproduceCooldown(0.0f),   
          colorTimer(0.f)
    {}

    virtual ~Agent() = default;
    void timeUntilDeath(float dt, float rate)
    {
        lifeTime -= dt * rate;
    }

    sf::Vector2i cell;
    float lifeTime;
    float stepInterval;
    float size;
    sf::Color color;
    sf::Color originalColor;
    float timeSinceMove;
    float reproduceCooldown;       
    inline static constexpr float defaultReproduceCooldown = 5.0f; 
    float colorTimer;
};

class AgentA : public Agent {
public:
    AgentA(sf::Vector2i c, float spd = 1.0f)
        : Agent(c, spd, sf::Color(106, 163, 217))
    {}
};

class AgentB : public Agent {
public:
    AgentB(sf::Vector2i c, float spd = 1.0f)
        : Agent(c, spd, sf::Color(198, 118, 232))
    {}
};
