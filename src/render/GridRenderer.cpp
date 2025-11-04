#include "render/GridRenderer.hpp"
#include "render/GridRenderer.hpp"
#include <algorithm>

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

    sf::View view = window.getView();
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size   = view.getSize();

    float left   = center.x - size.x * 0.5f;
    float top    = center.y - size.y * 0.5f;
    float right  = left + size.x;
    float bottom = top  + size.y;

    int startX = std::max(0, static_cast<int>(left  / cell));
    int startY = std::max(0, static_cast<int>(top   / cell));
    int endX   = std::min(state.gridWidth  - 1, static_cast<int>(right  / cell) + 1);
    int endY   = std::min(state.gridHeight - 1, static_cast<int>(bottom / cell) + 1);

    cellShape.setSize({cell, cell});

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            cellShape.setPosition({x * cell, y * cell});
            window.draw(cellShape);
        }
    }

    for (const auto& agent : state.agents) {
        const auto& cellPos = agent->cell;

        agentShape.setSize({cell - 1.0f, cell - 1.0f});
        agentShape.setFillColor(agent->color);
        agentShape.setPosition({cellPos.x * cell, cellPos.y * cell});
        window.draw(agentShape);
    }
}


