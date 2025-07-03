#pragma once

#include "CellsDefinitions/CellBuffer.h"
#include <CellsDefinitions/TypeIndexData.h>

struct SimulationContext 
{
    CellBuffer* _currentCellBuffer;
    CellBuffer* _nextCellBuffer;
    TypeIndexData* _indicesCurrent;
    TypeIndexData* _indicesNext;
    uint8_t randomOffset;
};