#pragma once

#include "CellsDefinitions/ECellTypes.h"
#include "SimulationContext.h"
#include <array>

namespace CellSimulationRegistry 
{

    using SimulateFunc = void(*)(SimulationContext&);

    void Init();
    void Simulate(ECellTypes type, SimulationContext& ctx);
    bool IsActive(ECellTypes type);
}