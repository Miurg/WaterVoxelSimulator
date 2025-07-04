#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace DownMove
{
    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx.indicesActiveCurrent.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesActiveCurrent[index];
            Cell currentCell = ctx.currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

            uint_fast16_t belowIndex = currentIndex - CHUNK_EXT;
            Cell belowCell = ctx.currentCellBuffer->Cells[belowIndex];

            if (!belowCell.IsAir() || belowCell.IsReserved()) continue;

            ctx.nextCellBuffer->Cells[belowIndex] = currentCell;
            ctx.nextCellBuffer->Cells[currentIndex] = belowCell;
            ctx.indicesNext->list[ctx.activeIndicesInIndicesCurrent[index]] = belowIndex;
            ctx.indicesNext->set.erase(currentIndex);
            ctx.indicesNext->set.insert(belowIndex);
            ctx.currentCellBuffer->Cells[belowIndex].SetReserved(true);
            ctx.currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
        }
    }
}
