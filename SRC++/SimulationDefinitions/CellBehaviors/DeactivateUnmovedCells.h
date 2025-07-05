#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace DeactivateUnmovedCells
{
	inline void Simulate(SimulationContext& ctx)
	{
        for (uint_fast16_t index = 0; index < ctx.indicesActiveCurrent.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesActiveCurrent[index];
            Cell currentCell = ctx.currentCellBuffer->Cells[currentIndex];

            if (currentCell.IsAlreadyMove() || !currentCell.IsActive()) continue;

            currentCell.SetActive(false);
            SetDirection(currentCell.Flags, static_cast<EDirection>(EDirection::DOWN));
            ctx.nextCellBuffer->Cells[currentIndex] = currentCell;

        }
	}
}
