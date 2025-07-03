#pragma once

#include "CellsDefinitions/ECellTypes.h"

struct CellVisual
{
    ECellTypes Type;

    CellVisual()
    {
        Type = ECellTypes::AIR;
    }
};