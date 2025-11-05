#pragma once
#include <imgui.h>
#include <SFML/System/Time.hpp>
#include "core/SimulationSystem.hpp"

class DebugPanel {
public:
    void draw(sf::Time dt, const SimulationSystem& sim);
};
