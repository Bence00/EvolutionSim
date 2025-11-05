#include "App.hpp"
#include <stdexcept>

App::App()
    : window(sf::VideoMode({gWindowWidth, gWindowHeight}), "Evo sim"),
      world1{},
      sim(world1),
      renderer(world1),
      simPanel(sim, world1),
      debugPanel(),
      view(window.getDefaultView())
{
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        throw std::runtime_error("imgui-sfml init failed");

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.8f;  
    ImGui::GetStyle().ScaleAllSizes(1.8f);
    window.setView(view);
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

        if (!ImGui::GetIO().WantCaptureMouse) {

            if (auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    panning = true;

                    oldMousePosWorld = window.mapPixelToCoords(
                        {mb->position.x, mb->position.y},
                        view
                    );
                }
            }
            else if (auto* mr = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mr->button == sf::Mouse::Button::Left) {
                    panning = false;
                }
            }
            else if (auto* mm = event->getIf<sf::Event::MouseMoved>()) {
                if (panning) {
                    sf::Vector2f newMousePosWorld = window.mapPixelToCoords(
                        {mm->position.x, mm->position.y},
                        view
                    );

                    sf::Vector2f delta = oldMousePosWorld - newMousePosWorld;
                    view.move(delta);

                    oldMousePosWorld = window.mapPixelToCoords(
                        {mm->position.x, mm->position.y},
                        view
                    );
                }
            }
            else if (auto* ws = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (ws->wheel == sf::Mouse::Wheel::Vertical) {
                    if (ws->delta > 0)
                        view.zoom(0.9f);   
                    else if (ws->delta < 0)
                        view.zoom(1.1f);                   }
            }
        }
    }
}


void App::update() {
    auto dt = clock.restart();
    ImGui::SFML::Update(window, dt);

    sim.update(dt);

    simPanel.draw();
    debugPanel.draw(dt, sim);
}

void App::render() {
    window.clear();

    window.setView(view);
    renderer.draw(window);   

    window.setView(window.getDefaultView());
    ImGui::SFML::Render(window);

    window.display();
}
