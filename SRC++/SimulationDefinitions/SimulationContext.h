#pragma once

#include "CellsDefinitions/CellBuffer.h"
#include <vector>
#include "Chunk.h"

struct SimulationContext {
    CellBuffer* _currentCellBuffer;
    CellBuffer* _nextCellBuffer;
    TypeIndexData* _indicesCurrent;
    TypeIndexData* _indicesNext;
    uint8_t randomOffset;
};