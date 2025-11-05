#include "render/GridRenderer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <algorithm>

GridRenderer::GridRenderer(WorldState& s)
    : state(s)
{
    m_gridFills.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_gridLines.setPrimitiveType(sf::PrimitiveType::Lines);
    m_agentQuads.setPrimitiveType(sf::PrimitiveType::Triangles);
}

void GridRenderer::draw(sf::RenderWindow& window)
{
    const float cell = static_cast<float>(state.cellSize);
    if (cell <= 0.f || state.gridWidth == 0 || state.gridHeight == 0)
        return;

    sf::View view = window.getView();
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size   = view.getSize();

    float left   = center.x - size.x * 0.5f;
    float top    = center.y - size.y * 0.5f;
    float right  = left + size.x;
    float bottom = top  + size.y;

    int startX = std::max(0, static_cast<int>(left / cell) - 1);
    int startY = std::max(0, static_cast<int>(top  / cell) - 1);
    int endX   = std::min(state.gridWidth,  static_cast<int>(right  / cell) + 2);
    int endY   = std::min(state.gridHeight, static_cast<int>(bottom / cell) + 2);

    //
    // 1) GRID FILL
    //
    m_gridFills.clear();
    const sf::Color fillColor(40, 40, 40);

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            float x1 = x * cell;
            float y1 = y * cell;
            float x2 = x1 + cell;
            float y2 = y1 + cell;

            // 4 corners
            sf::Vertex v1, v2, v3, v4;
            v1.position = {x1, y1};
            v2.position = {x2, y1};
            v3.position = {x2, y2};
            v4.position = {x1, y2};
            v1.color = v2.color = v3.color = v4.color = fillColor;

            // two triangles
            m_gridFills.append(v1);
            m_gridFills.append(v2);
            m_gridFills.append(v3);

            m_gridFills.append(v1);
            m_gridFills.append(v3);
            m_gridFills.append(v4);
        }
    }

    //
    // 2) GRID LINES
    //
    m_gridLines.clear();
    const sf::Color lineColor(60, 60, 60);

    float gridLeft   = startX * cell;
    float gridRight  = endX   * cell;
    float gridTop    = startY * cell;
    float gridBottom = endY   * cell;

    // horizontal
    for (int y = startY; y <= endY; ++y) {
        float yPos = y * cell;

        sf::Vertex v1, v2;
        v1.position = {gridLeft,  yPos};
        v2.position = {gridRight, yPos};
        v1.color = v2.color = lineColor;

        m_gridLines.append(v1);
        m_gridLines.append(v2);
    }

    // vertical
    for (int x = startX; x <= endX; ++x) {
        float xPos = x * cell;

        sf::Vertex v1, v2;
        v1.position = {xPos, gridTop};
        v2.position = {xPos, gridBottom};
        v1.color = v2.color = lineColor;

        m_gridLines.append(v1);
        m_gridLines.append(v2);
    }

    //
    // 3) AGENTS
    //
    m_agentQuads.clear();

    const float agentMargin = 0.5f;
    const float agentSize   = std::max(0.f, cell - agentMargin * 2.f);

    for (const auto& agent : state.agents) {
        const auto& cellPos = agent->cell;

        // culling agentre is
        if (cellPos.x < startX || cellPos.x >= endX ||
            cellPos.y < startY || cellPos.y >= endY) {
            continue;
        }

        float x1 = cellPos.x * cell + agentMargin;
        float y1 = cellPos.y * cell + agentMargin;
        float x2 = x1 + agentSize;
        float y2 = y1 + agentSize;

        sf::Vertex a1, a2, a3, a4;
        a1.position = {x1, y1};
        a2.position = {x2, y1};
        a3.position = {x2, y2};
        a4.position = {x1, y2};
        a1.color = a2.color = a3.color = a4.color = agent->color;

        // two triangles
        m_agentQuads.append(a1);
        m_agentQuads.append(a2);
        m_agentQuads.append(a3);

        m_agentQuads.append(a1);
        m_agentQuads.append(a3);
        m_agentQuads.append(a4);
    }

    //
    // 4) DRAW
    //
    window.draw(m_gridFills);
    window.draw(m_gridLines);
    window.draw(m_agentQuads);
}
