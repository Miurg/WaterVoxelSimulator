#pragma once

#include "CellsDefinitions/CellTypes.h"
#include "SimulationContext.h"
#include <array>

namespace CellSimulationRegistry {

    using SimulateFunc = void(*)(SimulationContext&);

    void Init();
    void Simulate(CellTypes type, SimulationContext& ctx);
    bool IsActive(CellTypes type);
}