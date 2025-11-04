#include "App.hpp"
#include <stdexcept>

App::App()
    : window(sf::VideoMode({1200, 1200}), "Evo sim"),
      state{},
      sim(state),
      renderer(state),
      simPanel(sim, state),
      debugPanel()
{
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        throw std::runtime_error("imgui-sfml init failed");

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.8f;  
    ImGui::GetStyle().ScaleAllSizes(1.8f);
}

App::~App() {
    ImGui::SFML::Shutdown();
}

void App::run() {
    while (running && window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void App::processEvents() {
    while (auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void App::update() {
    auto dt = clock.restart();
    ImGui::SFML::Update(window, dt);

    sim.update(dt);

    simPanel.draw();
    debugPanel.draw(dt);
}

void App::render() {
    window.clear(sf::Color::Black);
    renderer.draw(window);      
    ImGui::SFML::Render(window);
    window.display();
}
