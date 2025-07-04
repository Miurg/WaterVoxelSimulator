#include "CellSimulationRegistry.h"
#include <cassert>
#include "CellsDefinitions/ECellTypes.h"
#include "SimulationDefinitions/CellBehaviors/LiquidMove.h"
#include "SimulationDefinitions/CellBehaviors/DownMove.h"
#include "SimulationDefinitions/CellBehaviors/DeactivateUnmovedCells.h"
#include "SimulationDefinitions/CellBehaviors/ActivateNeighborsOfMovedCells.h"
#include "SimulationDefinitions/CellBehaviors/FilterOutDeadLayerCells.h"
#include "SimulationDefinitions/CellBehaviors/GatherActiveCellIndices.h"

#include "SimulationDefinitions/CellBehaviors/PredetermentMove.h"

static void SimulateAir(SimulationContext& ctx) 
{
    // ...
}
static void SimulateWater(SimulationContext& ctx) 
{
    //=== Preprocess ===
    GatherActiveCellIndices::Simulate(ctx);

    //=== Main behavior ===
    DownMove::Simulate(ctx);
    PredetermentMove::Simulate(ctx);
    LiquidMove::Simulate(ctx);

    //=== Postprocess ===
    DeactivateUnmovedCells::Simulate(ctx);
    ActivateNeighborsOfMovedCells::Simulate(ctx);
    FilterOutDeadLayerCells::Simulate(ctx);
}

static void SimulateSand(SimulationContext& ctx) 
{

}

static void SimulateDirt(SimulationContext& ctx) 
{

}

namespace CellSimulationRegistry 
{

    static std::array<SimulateFunc, static_cast<size_t>(ECellTypes::COUNT)> simulateFuncs;
    static std::array<bool, static_cast<size_t>(ECellTypes::COUNT)> isActiveFlags;

    void Init() 
    {
        simulateFuncs = 
        {
            SimulateAir,
            SimulateSand,
            SimulateWater,
            SimulateDirt
        };

        isActiveFlags = 
        {
            false,  //Air
            true,   //Sand
            true,   //Water
            false   //Dirt
        };
    }

    void Simulate(ECellTypes type, SimulationContext& ctx) 
    {
        auto index = static_cast<size_t>(type);
        assert(index < simulateFuncs.size());
        simulateFuncs[index](ctx);
    }

    bool IsActive(ECellTypes type) 
    {
        return isActiveFlags[static_cast<size_t>(type)];
    }

}