#pragma once
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

#include "core/WorldState.hpp"
#include "core/SimulationSystem.hpp"
#include "render/GridRenderer.hpp"
#include "ui/SimulationPanel.hpp"
#include "ui/DebugPanel.hpp"

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

    WorldState state;
    SimulationSystem sim;
    GridRenderer renderer;
    SimulationPanel simPanel;
    DebugPanel debugPanel;

    bool running = true;
};
