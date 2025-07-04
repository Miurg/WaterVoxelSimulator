#pragma once

#include "SimulationDefinitions/SimulationContext.h"
#include <CellsDefinitions/TypeIndexData.h>

namespace FilterOutDeadLayerCells
{
    inline void Simulate(SimulationContext& ctx)
    {
        TypeIndexData& data = *ctx.indicesNext;
        auto& vec = data.list;
        auto& set = data.set;
        uint_fast16_t writeIndex = 0;
        for (uint_fast16_t readIndex = 0; readIndex < vec.size(); ++readIndex)
        {
            uint_fast16_t index = vec[readIndex];
            if (!IsDeadLayerCell(index))
            {
                vec[writeIndex++] = index;
            }
            else
            {
                set.erase(index);
            }
        }
        vec.resize(writeIndex);
    }
}
