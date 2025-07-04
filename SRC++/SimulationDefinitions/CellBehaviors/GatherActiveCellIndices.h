#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include <CellsDefinitions/TypeIndexData.h>

namespace GatherActiveCellIndices
{
    inline void Simulate(SimulationContext& ctx)
    {
        for (uint_fast16_t index = 0; index < ctx.indicesCurrent->list.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx.indicesCurrent->list[index];
            if (ctx.currentCellBuffer->Cells[currentIndex].IsActive())
            {
                ctx.indicesActiveCurrent.push_back(currentIndex);
                ctx.activeIndicesInIndicesCurrent.push_back(index);
            }
        }
    }
}
