#pragma once

#include <cstdint>
#include "CellsDefinitions/CellTypes.h"

struct CellVisual
{
    uint16_t IndexPosition;
    CellTypes Type;

    CellVisual(uint16_t Position, CellTypes type)
    {
        IndexPosition = Position;
        Type = type;
    }
};