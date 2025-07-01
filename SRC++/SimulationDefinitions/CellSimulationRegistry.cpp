#include "CellSimulationRegistry.h"
#include <cassert>
#include "CellsDefinitions/CellTypes.h"
#include "Chunk.h"
#include "LiquidMove.h"
#include "SimulationConst.h"
#include <xmmintrin.h>
#include "DownMove.h"
#include "DeactivateUnmovedCells.h"
#include "ActivateNeighborsOfMovedCells.h"
#include "FilterOutDeadLayerCells.h"
static void SimulateAir(SimulationContext& ctx) 
{
    // ...
}
static void SimulateWater(SimulationContext& ctx) 
{
    DownMove::Simulate(ctx);
    LiquidMove::Simulate(ctx);

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

    static std::array<SimulateFunc, static_cast<size_t>(CellTypes::COUNT)> simulateFuncs;
    static std::array<bool, static_cast<size_t>(CellTypes::COUNT)> isActiveFlags;

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

    void Simulate(CellTypes type, SimulationContext& ctx) 
    {
        auto index = static_cast<size_t>(type);
        assert(index < simulateFuncs.size());
        simulateFuncs[index](ctx);
    }

    bool IsActive(CellTypes type) 
    {
        return isActiveFlags[static_cast<size_t>(type)];
    }

}