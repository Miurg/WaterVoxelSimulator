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

    inline bool TryMove(uint16_t currentIndex, Cell currentCell, int offset, uint16_t indexInMassive, SimulationContext& ctx)
    {
        int targetIndex = currentIndex + offset;
        Cell targetCell = ctx._currentCellBuffer->Cells[targetIndex];
        if (!targetCell.IsAir() || targetCell.IsReserved()) return false;

        ctx._nextCellBuffer->Cells[targetIndex] = currentCell;
        ctx._nextCellBuffer->Cells[currentIndex] = targetCell;
        ctx._indicesNext->list[indexInMassive] = targetIndex;
        ctx._indicesNext->set.erase(currentIndex);
        ctx._indicesNext->set.insert(targetIndex);
        ctx._currentCellBuffer->Cells[targetIndex].SetReserved(true);
        ctx._currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
        return true;
    }

    inline uint8_t Simulate(uint16_t index, SimulationContext& ctx, uint8_t offsetIndexStart)
    {
        uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
        Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];
        if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) return offsetIndexStart;

        auto now = (offsetIndexStart + 1) & 3;

        if (TryMove(currentIndex, currentCell, _offsets[now], index, ctx)) return now;
        now = (offsetIndexStart + 2) & 3;
        if (TryMove(currentIndex, currentCell, _offsets[now], index, ctx)) return now;
        now = (offsetIndexStart + 3) & 3;
        if (TryMove(currentIndex, currentCell, _offsets[now], index, ctx)) return now;
        now = (offsetIndexStart + 4) & 3;
        if (TryMove(currentIndex, currentCell, _offsets[now], index, ctx)) return now;
        return offsetIndexStart;
    }
}        