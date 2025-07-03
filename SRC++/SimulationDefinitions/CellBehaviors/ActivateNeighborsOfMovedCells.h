#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace ActivateNeighborsOfMovedCells
{
    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx._indicesCurrent->list.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
            if (!ctx._currentCellBuffer->Cells[currentIndex].IsAlreadyMove()) continue;

            ctx._nextCellBuffer->Cells[currentIndex + 1].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex - 1].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex + CHUNK_EXT].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex - CHUNK_EXT].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex + CHUNK_EXT2].SetActive(true);
            ctx._nextCellBuffer->Cells[currentIndex - CHUNK_EXT2].SetActive(true);
        }
    }
}