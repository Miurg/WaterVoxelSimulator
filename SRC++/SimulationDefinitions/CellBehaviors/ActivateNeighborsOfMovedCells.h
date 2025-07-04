#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace ActivateNeighborsOfMovedCells
{
    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx.indicesActiveCurrent.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesActiveCurrent[index];
            if (!ctx.currentCellBuffer->Cells[currentIndex].IsAlreadyMove()) continue;

            ctx.nextCellBuffer->Cells[currentIndex + 1].SetActive(true);
            ctx.nextCellBuffer->Cells[currentIndex - 1].SetActive(true);
            ctx.nextCellBuffer->Cells[currentIndex + CHUNK_EXT].SetActive(true);
            ctx.nextCellBuffer->Cells[currentIndex - CHUNK_EXT].SetActive(true);
            ctx.nextCellBuffer->Cells[currentIndex + CHUNK_EXT2].SetActive(true);
            ctx.nextCellBuffer->Cells[currentIndex - CHUNK_EXT2].SetActive(true);
        }
    }
}