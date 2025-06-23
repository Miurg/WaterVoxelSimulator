#include "CellSimulationRegistry.h"
#include <cassert>
#include "CellsDefinitions/CellTypes.h"
#include "Chunk.h"
#include "LiquidMove.h"
#include "SimulationConst.h"
#include <xmmintrin.h>

static void SimulateAir(SimulationContext& ctx) {
    // ...
}
static void SimulateWater(SimulationContext& ctx) 
{
    for (uint_fast16_t index = 0; index < ctx.indicesCount; ++index)
    {
        uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];

        Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];

        if (!currentCell.IsActive()) continue;

        uint_fast16_t belowIndex = currentIndex - CHUNK_EXT;
        Cell belowCell = ctx._currentCellBuffer->Cells[belowIndex];

        if (!belowCell.IsAir() || belowCell.IsReserved()) continue;

        ctx._nextCellBuffer->Cells[belowIndex] = currentCell;
        ctx._nextCellBuffer->Cells[currentIndex] = belowCell;
        ctx._indicesNext->list[index] = belowIndex;
        ctx._indicesNext->set.erase(currentIndex);
        ctx._indicesNext->set.insert(belowIndex);
        ctx._currentCellBuffer->Cells[belowIndex].SetReserved(true);
        ctx._currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
    }        
    
    uint8_t currentOffsetIndex = ctx.randomOffset;
    for (uint_fast16_t index = 0; index < ctx.indicesCount; ++index)
    {
        uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
        currentOffsetIndex = LiquidMove::Simulate(index,ctx, (currentOffsetIndex+currentIndex));
        Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];
        if (currentCell.IsAir()) continue;
        if (!currentCell.IsActive())
        {
            ctx._nextCellBuffer->Cells[currentIndex] = currentCell;
        }
        else if (!currentCell.IsAlreadyMove())
        {
            currentCell.SetActive(false);
            ctx._nextCellBuffer->Cells[currentIndex] = currentCell;
        }
    }
    ctx.randomOffset = currentOffsetIndex;

    for (uint_fast16_t index = 0; index < ctx.indicesCount; ++index)
    {
        uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
        if (ctx._currentCellBuffer->Cells[currentIndex].IsAlreadyMove())
        {
            ctx._nextCellBuffer->Cells[currentIndex + 1].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex - 1] .SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex + CHUNK_EXT].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex - CHUNK_EXT].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex + CHUNK_EXT2].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex - CHUNK_EXT2].SetActive(true);
        }
    }
    
    TypeIndexData& data = *ctx._indicesNext;
    auto& vec = data.list;
    auto& set = data.set;

    uint_fast16_t writeIndex = 0;
    for (uint_fast16_t readIndex = 0; readIndex < vec.size(); ++readIndex) 
    {
        uint_fast16_t index = vec[readIndex];
        if (!IsDeadLayerCell(index)) 
        {
            vec[writeIndex++] = index;
        }
        else 
        {
            set.erase(index); // удалить мёртвого из множества
        }
    }
    vec.resize(writeIndex);
}
static void SimulateSand(SimulationContext& ctx) 
{
    //for (uint_fast16_t index = 0; index < ctx.indicesCount; index++)
    //{
    //    uint_fast16_t currentIndex = ctx._indicesCurrent->at(index);
    //    Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];
    //    ctx._nextCellBuffer->Cells[currentIndex] = currentCell;
    //    ctx._indicesNext->at(index) = currentIndex;
    //}
}
static void SimulateDirt(SimulationContext& ctx) 
{
   /* for (uint_fast16_t index = 0; index < ctx.indicesCount; index++)
    {
        uint_fast16_t currentIndex = ctx._indicesCurrent->at(index);
        Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];
        ctx._nextCellBuffer->Cells[currentIndex] = currentCell;
        ctx._indicesNext->at(index) = currentIndex;
    }*/

}
namespace CellSimulationRegistry {

    static std::array<SimulateFunc, static_cast<size_t>(CellTypes::COUNT)> simulateFuncs;
    static std::array<bool, static_cast<size_t>(CellTypes::COUNT)> isActiveFlags;

    void Init() {
        simulateFuncs = {
            SimulateAir,
            SimulateSand,
            SimulateWater,
            SimulateDirt
        };

        isActiveFlags = {
            false,  //Air
            true,   //Sand
            true,   //Water
            false   //Dirt
        };
    }

    void Simulate(CellTypes type, SimulationContext& ctx) {
        auto index = static_cast<size_t>(type);
        assert(index < simulateFuncs.size());
        simulateFuncs[index](ctx);
    }

    bool IsActive(CellTypes type) {
        return isActiveFlags[static_cast<size_t>(type)];
    }

}