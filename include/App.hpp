#pragma once
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

#include "core/WorldState.hpp"
#include "core/SimulationSystem.hpp"
#include "render/GridRenderer.hpp"
#include "ui/SimulationPanel.hpp"
#include "ui/DebugPanel.hpp"
#include "globals.hpp"
#include "ui/PlotPanel.hpp"

class App {
public:
    App();
    ~App();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    sf::Clock clock;

    WorldState world1;
    SimulationSystem sim;
    GridRenderer renderer;
    SimulationPanel simPanel;
    DebugPanel debugPanel;
    PlotPanel plotPanel;

    sf::View view;
    sf::Vector2f oldMousePosWorld;

    bool panning = false;
    bool running = true;
};
