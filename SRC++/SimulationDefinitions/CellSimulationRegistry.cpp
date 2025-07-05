#include "CellSimulationRegistry.h"
#include <cassert>
#include "CellsDefinitions/ECellTypes.h"
#include "SimulationDefinitions/CellBehaviors/MoveCells.h"
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
    std::vector<EDirection> directions;
    directions.push_back(EDirection::DOWN);

    MoveCells::Simulate(ctx, directions, false);
    PredetermentMove::Simulate(ctx);

    directions.clear();
    directions.push_back(EDirection::BACKWARD);
    directions.push_back(EDirection::FORWARD);
    directions.push_back(EDirection::LEFT);
    directions.push_back(EDirection::RIGHT);
    directions.push_back(EDirection::BACKWARD_LEFT);
    directions.push_back(EDirection::BACKWARD_RIGHT);
    directions.push_back(EDirection::FORWARD_LEFT);
    directions.push_back(EDirection::FORWARD_RIGHT);

    MoveCells::Simulate(ctx, directions, true);

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