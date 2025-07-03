#pragma once

#include "CellsDefinitions/ECellTypes.h"
#include "SimulationContext.h"

namespace CellSimulationRegistry 
{

    using SimulateFunc = void(*)(SimulationContext&);

    void Init();
    void Simulate(ECellTypes type, SimulationContext& ctx);
    bool IsActive(ECellTypes type);
}