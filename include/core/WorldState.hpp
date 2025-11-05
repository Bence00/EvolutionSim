#pragma once
#include <memory>
#include <vector>
#include "core/Agent.hpp"

struct WorldState {
    int gridWidth  = 21;
    int gridHeight = 41;
    int cellSize   = 15;

    std::vector<std::unique_ptr<Agent>> agents;
};
