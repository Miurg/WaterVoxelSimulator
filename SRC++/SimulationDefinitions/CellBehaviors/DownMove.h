#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace DownMove
{
    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx._indicesCurrent->list.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
            Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

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
    }
}
