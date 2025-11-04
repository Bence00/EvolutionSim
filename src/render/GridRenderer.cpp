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
    const float cell = static_cast<float>(state.cellSize);

    // grid
    cellShape.setSize({cell, cell});
    for (int y = 0; y < state.gridHeight; ++y) {
        for (int x = 0; x < state.gridWidth; ++x) {
            cellShape.setPosition({x * cell, y * cell});
            window.draw(cellShape);
        }
    }

    // agents
    for (const auto& agent : state.agents) {
        const auto& cellPos = agent->cell;

        agentShape.setSize({cell-1.0f, cell-1.0f});
        agentShape.setFillColor(agent->color);

        agentShape.setPosition({cellPos.x * cell, cellPos.y * cell});
        window.draw(agentShape);
    }
}

