#pragma once

#include "core/WorldState.hpp"
#include <SFML/Graphics/VertexArray.hpp>

namespace sf {
    class RenderWindow;
}

class GridRenderer {
public:
    explicit GridRenderer(WorldState& s);

    void draw(sf::RenderWindow& window);

private:
    WorldState& state;

    sf::VertexArray m_gridFills;   
    sf::VertexArray m_gridLines;   
    sf::VertexArray m_agentQuads;  
};
