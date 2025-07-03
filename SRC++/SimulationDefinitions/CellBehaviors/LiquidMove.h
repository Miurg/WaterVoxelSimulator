#pragma once

#include "CellsDefinitions/ECellTypes.h"
#include "SimulationDefinitions/SimulationContext.h"
#include <array>
#include <chrono>
#include "SimulationDefinitions/CellSimulationRegistry.h"
#include <cassert>
#include "SimulationDefinitions/SimulationConst.h"
#include "Chunk.h"

namespace LiquidMove 
{

    static constexpr int _offsets[4] = { 1, -1, CHUNK_EXT2, -CHUNK_EXT2 };//dx,dz

    inline bool TryMove(uint16_t currentIndex, Cell *currentCell, uint8_t direction, uint16_t indexInMassive, SimulationContext& ctx)
    {
        uint16_t offsetByDirection = _offsets[direction];
        uint16_t targetIndex = currentIndex + offsetByDirection;
        Cell targetCell = ctx._currentCellBuffer->Cells[targetIndex];
        if (!targetCell.IsAir() || targetCell.IsReserved()) return false;

        ctx._nextCellBuffer->Cells[targetIndex] = *currentCell;
        ctx._nextCellBuffer->Cells[currentIndex] = targetCell;
        ctx._indicesNext->list[indexInMassive] = targetIndex;
        ctx._indicesNext->set.erase(currentIndex);
        ctx._indicesNext->set.insert(targetIndex);
        ctx._currentCellBuffer->Cells[targetIndex].SetReserved(true);
        ctx._currentCellBuffer->Cells[currentIndex].SetAlreadyMove(true);
        SetDirection(ctx._nextCellBuffer->Cells[targetIndex].Flags, static_cast<EDirection>(direction));
        return true;
    }

    inline void Simulate(SimulationContext& ctx)
    {
        uint8_t offsetIndexStart = ctx.randomOffset;
        for (uint_fast16_t index = 0; index < ctx._indicesCurrent->list.size(); ++index)
        {
            uint_fast16_t currentIndex = ctx._indicesCurrent->list[index];
            Cell currentCell = ctx._currentCellBuffer->Cells[currentIndex];

            if (!currentCell.IsActive() || currentCell.IsAlreadyMove()) continue;

            uint8_t predetermentMove = static_cast<uint8_t>(GetDirection(currentCell.Flags));

            if (TryMove(currentIndex, &currentCell, predetermentMove, index, ctx)) continue;
            offsetIndexStart += predetermentMove;
            if (TryMove(currentIndex, &currentCell, offsetIndexStart & 3, index, ctx)) continue;
            offsetIndexStart += 1;
            if (TryMove(currentIndex, &currentCell, offsetIndexStart & 3, index, ctx)) continue;
            offsetIndexStart += 2;
            if (TryMove(currentIndex, &currentCell, offsetIndexStart & 3, index, ctx)) continue;
        }
        ctx.randomOffset = offsetIndexStart;
    }
}        