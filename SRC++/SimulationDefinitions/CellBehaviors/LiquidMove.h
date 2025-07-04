#pragma once
#include "SimulationDefinitions/SimulationContext.h"
#include "SimulationDefinitions/SimulationConst.h"

namespace LiquidMove 
{

    static constexpr int _offsets[4] = { 1, -1, CHUNK_EXT2, -CHUNK_EXT2 };//dx,dz

    inline bool TryMove(uint_fast16_t currentIndex, Cell *currentCell, uint_fast8_t direction, uint_fast16_t indexInMassive, SimulationContext& ctx)
    {
        uint_fast16_t offsetByDirection = _offsets[direction];
        uint_fast16_t targetIndex = currentIndex + offsetByDirection;
        Cell targetCell = ctx.currentCellBuffer->Cells[targetIndex];
        if (!targetCell.IsAir() || targetCell.IsReserved()) return false;

        ctx.nextCellBuffer->Cells[targetIndex] = *currentCell;
        ctx.nextCellBuffer->Cells[currentIndex] = targetCell;
        ctx.indicesNext->list[ctx.activeIndicesInIndicesCurrent[indexInMassive]] = targetIndex;
        ctx.indicesNext->set.erase(currentIndex);
        ctx.indicesNext->set.insert(targetIndex);
        ctx.currentCellBuffer->Cells[targetIndex].SetReserved(true);
        ctx.currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
        SetDirection(ctx.nextCellBuffer->Cells[targetIndex].Flags, static_cast<EDirection>(direction));
        return true;
    }

    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx.indicesActiveCurrent.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesActiveCurrent[index];
            Cell currentCell = ctx.currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

            int random_index = ctx.dist(ctx.gen);
            if (TryMove(currentIndex, &currentCell, random_index & 3, index, ctx)) continue;
            random_index += 1;
            if (TryMove(currentIndex, &currentCell, random_index & 3, index, ctx)) continue;
            random_index += 2;
            if (TryMove(currentIndex, &currentCell, random_index & 3, index, ctx)) continue;
            random_index += 3;
            if (TryMove(currentIndex, &currentCell, random_index & 3, index, ctx)) continue;
        }
    }
}        