#pragma once
#include <SFML/Graphics.hpp>
#include "core/WorldState.hpp"

class GridRenderer {
public:
    explicit GridRenderer(WorldState& state);

    void draw(sf::RenderWindow& window);

private:
    WorldState& state;

    sf::RectangleShape cellShape;  
    sf::RectangleShape agentShape;    
};
