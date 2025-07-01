#pragma once

#include "SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace DeactivateUnmovedCells
{
	inline void Simulate(SimulationContext& ctx)
	{
        for (uint_fast16_t index = 0; index < ctx._indicesCurrent->list.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
            Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];

            if (currentCell.IsAlreadyMove() || !currentCell.IsActive()) continue;

            currentCell.SetActive(false);
            ctx._nextCellBuffer->Cells[currentIndex] = currentCell;
        }
	}
}
