#pragma once

#include "CellsDefinitions/CellTypes.h"
#include "SimulationContext.h"
#include <array>
#include <chrono>
#include "CellSimulationRegistry.h"
#include <cassert>
#include "SimulationConst.h"
#include "Chunk.h"

namespace LiquidMove 
{

    static constexpr int _offsets[4] = { 1, -1, CHUNK_EXT2, -CHUNK_EXT2 };//dx,dz

    inline bool TryMove(uint16_t currentIndex, Cell *currentCell, int offset, uint16_t indexInMassive, SimulationContext& ctx)
    {
        int targetIndex = currentIndex + offset;
        Cell targetCell = ctx._currentCellBuffer->Cells[targetIndex];
        if (!targetCell.IsAir() || targetCell.IsReserved()) return false;

        ctx._nextCellBuffer->Cells[targetIndex] = *currentCell;
        ctx._nextCellBuffer->Cells[currentIndex] = targetCell;
        ctx._indicesNext->list[indexInMassive] = targetIndex;
        ctx._indicesNext->set.erase(currentIndex);
        ctx._indicesNext->set.insert(targetIndex);
        ctx._currentCellBuffer->Cells[targetIndex].SetReserved(true);
        ctx._currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
        currentCell->SetReserved(true);
        currentCell->SetAlreadyMove(true);
        return true;
    }

    inline void Simulate(SimulationContext& ctx)
    {
        uint8_t offsetIndexStart = ctx.randomOffset;
        for (uint_fast16_t index = 0; index < ctx._indicesCurrent->list.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
            Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

            offsetIndexStart = (offsetIndexStart + index);

            if (TryMove(currentIndex, &currentCell, _offsets[offsetIndexStart & 3], index, ctx)) continue;
            offsetIndexStart = (offsetIndexStart + 1);
            if (TryMove(currentIndex, &currentCell, _offsets[offsetIndexStart & 3], index, ctx)) continue;
            offsetIndexStart = (offsetIndexStart + 2);
            if (TryMove(currentIndex, &currentCell, _offsets[offsetIndexStart & 3], index, ctx)) continue;
            offsetIndexStart = (offsetIndexStart + 3);
            if (TryMove(currentIndex, &currentCell, _offsets[offsetIndexStart & 3], index, ctx)) continue;
        }
        ctx.randomOffset = offsetIndexStart;
    }
}        