#include "render/GridRenderer.hpp"

GridRenderer::GridRenderer(WorldState& s)
    : state(s)
{
    cellShape.setFillColor(sf::Color(40, 40, 40));
    cellShape.setOutlineColor(sf::Color(60, 60, 60));
    cellShape.setOutlineThickness(1.f);

    agentShape.setOrigin({0.f, 0.f});
}

void GridRenderer::draw(sf::RenderWindow& window)
{
    for (int y = 0; y < state.gridHeight; ++y) {
        for (int x = 0; x < state.gridWidth; ++x) {
            cellShape.setSize(sf::Vector2f(
                static_cast<float>(state.cellSize),
                static_cast<float>(state.cellSize)
            ));
            cellShape.setPosition(sf::Vector2f{
                static_cast<float>(x * state.cellSize),
                static_cast<float>(y * state.cellSize)
            });
            window.draw(cellShape);
        }
    }

    for (const auto& agent : state.agents) {
        const auto& cell = agent->cell;

        float s = agent->size * state.cellSize; 
        agentShape.setSize(sf::Vector2f(2*s, 2*s));
        
        agentShape.setFillColor(agent->color);

        float px = cell.x * state.cellSize + (state.cellSize - 2*s) * 0.5f;
        float py = cell.y * state.cellSize + (state.cellSize - 2*s) * 0.5f;

        agentShape.setPosition(sf::Vector2f(px, py));

        window.draw(agentShape);
    }
}
