#pragma once

#include <cstdint>
#include "CellsDefinitions/CellTypes.h"

struct CellVisual
{
    CellTypes Type;

    CellVisual()
    {
        Type = CellTypes::AIR;
    }
};