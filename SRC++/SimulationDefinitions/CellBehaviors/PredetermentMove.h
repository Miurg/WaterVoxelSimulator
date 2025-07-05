#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include "CellsDefinitions/CellBuffer.h"

namespace PredetermentMove
{
    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx.indicesActiveCurrent.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesActiveCurrent[index];
            Cell currentCell = ctx.currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

            uint_fast16_t offsetByDirection = OffsetsForDirections18[static_cast<uint8_t>(GetDirection(currentCell.Flags))];
            uint_fast16_t targetIndex = currentIndex + offsetByDirection;
            Cell predetermentCell = ctx.currentCellBuffer->Cells[targetIndex];

            if (!predetermentCell.IsAir() || predetermentCell.IsReserved()) continue;

            ctx.nextCellBuffer->Cells[targetIndex] = currentCell;
            ctx.nextCellBuffer->Cells[currentIndex] = predetermentCell;
            ctx.indicesNext->list[ctx.activeIndicesInIndicesCurrent[index]] = targetIndex;
            ctx.indicesNext->set.erase(currentIndex);
            ctx.indicesNext->set.insert(targetIndex);
            ctx.currentCellBuffer->Cells[targetIndex].SetReserved(true);
            ctx.currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
        }
    }
}
