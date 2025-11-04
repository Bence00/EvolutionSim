#pragma once
#include <memory>
#include <vector>
#include "core/Agent.hpp"

struct WorldState {
    int gridWidth  = 100;
    int gridHeight = 100;
    int cellSize   = 15;

    std::vector<std::unique_ptr<Agent>> agents;
};
