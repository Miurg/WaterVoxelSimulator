#pragma once
#include "SimulationDefinitions/SimulationContext.h"
#include "SimulationDefinitions/SimulationConst.h"

namespace MoveCells 
{
    inline void Simulate(SimulationContext& ctx, std::vector<EDirection> directions, bool needRememberDirection)
    {
        for (uint_fast16_t index = 0; index < ctx.indicesActiveCurrent.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesActiveCurrent[index];
            Cell currentCell = ctx.currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

            std::shuffle(directions.begin(), directions.end(), ctx.gen);
            for (int i = 0; i < directions.size(); ++i) 
            {
                int direction = static_cast<int>(directions[i]);
                uint_fast16_t targetIndex = currentIndex + OffsetsForDirections18[direction];
                Cell targetCell = ctx.currentCellBuffer->Cells[targetIndex];

                if (!targetCell.IsAir() || targetCell.IsReserved()) continue;

                ctx.nextCellBuffer->Cells[targetIndex] = currentCell;
                ctx.nextCellBuffer->Cells[currentIndex] = targetCell;
                ctx.indicesNext->list[ctx.activeIndicesInIndicesCurrent[index]] = targetIndex;
                ctx.indicesNext->set.erase(currentIndex);
                ctx.indicesNext->set.insert(targetIndex);
                ctx.currentCellBuffer->Cells[targetIndex].SetReserved(true);
                ctx.currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
                if (needRememberDirection) SetDirection(ctx.nextCellBuffer->Cells[targetIndex].Flags, static_cast<EDirection>(direction));
                break;
            }
        }
    }
}        