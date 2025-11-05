#pragma once
#include <memory>
#include <vector>
#include "core/Agent.hpp"

struct WorldState {
    int gridWidth  = 50;
    int gridHeight = 50;
    int cellSize   = 15;

    std::vector<std::unique_ptr<Agent>> agents;
};
