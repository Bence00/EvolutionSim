#pragma once
#include <memory>
#include <vector>
#include "core/Agent.hpp"

struct WorldState {
    int gridWidth  = 30;
    int gridHeight = 30;
    int cellSize   = 25;

    std::vector<std::unique_ptr<Agent>> agents;
};
