#pragma once

#include "CellsDefinitions/CellBuffer.h"
#include <CellsDefinitions/TypeIndexData.h>
#include <random>

struct SimulationContext 
{
    CellBuffer* currentCellBuffer;
    CellBuffer* nextCellBuffer;
    TypeIndexData* indicesCurrent;
    std::vector<uint_fast16_t> indicesActiveCurrent;
    std::vector<uint_fast16_t> activeIndicesInIndicesCurrent;
    TypeIndexData* indicesNext;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
};