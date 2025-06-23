#pragma once

#include <cstring>
#include "SimulationDefinitions/SimulationConst.h"
#include "CellFlags.h"
#include "CellTypes.h"

struct Cell
{
    CellTypes Type;
    CellFlags Flags;

    Cell() : Type(CellTypes::AIR), Flags(CellFlags::NONE) {}

    bool IsAir() const
    {
        return Type == CellTypes::AIR;
    }

    bool IsReserved() const
    {
        return (Flags & CellFlags::RESERVED) == CellFlags::RESERVED;
    }

    bool IsActive() const
    {
        return (Flags & CellFlags::ACTIVE) == CellFlags::ACTIVE;
    }

    bool IsAlreadyMove() const
    {
        return (Flags & CellFlags::HASMOVED) == CellFlags::HASMOVED;
    }

    void SetReserved(bool value)
    {
        if (value)
            Flags = static_cast<CellFlags>(Flags | CellFlags::RESERVED);
        else
            Flags = static_cast<CellFlags>(Flags & ~CellFlags::RESERVED);
    }

    void SetAlreadyMove(bool value)
    {
        if (value)
            Flags = static_cast<CellFlags>(Flags | CellFlags::HASMOVED);
        else
            Flags = static_cast<CellFlags>(Flags & ~CellFlags::HASMOVED);
    }

    void SetActive(bool value)
    {
        if (value)
            Flags = static_cast<CellFlags>(Flags | CellFlags::ACTIVE);
        else
            Flags = static_cast<CellFlags>(Flags & ~CellFlags::ACTIVE);
    }

    void SetType(CellTypes type)
    {
        Type = type;
    }
};

struct CellBuffer
{
    Cell Cells[CHUNK_EXT3];

    CellBuffer()
    {
        std::memset(Cells, 0, sizeof(Cells));
    }

    bool IsAirAt(uint16_t index) const
    {
        return Cells[index].Type == CellTypes::AIR;
    }

    bool IsReservedAt(uint16_t index) const
    {
        return (Cells[index].Flags & CellFlags::RESERVED) == CellFlags::RESERVED;
    }

    bool IsActiveAt(uint16_t index) const
    {
        return (Cells[index].Flags & CellFlags::ACTIVE) == CellFlags::ACTIVE;
    }

    bool IsAlreadyMoveAt(uint16_t index) const
    {
        return (Cells[index].Flags & CellFlags::HASMOVED) == CellFlags::HASMOVED;
    }

    void SetReservedAt(uint16_t index, bool value)
    {
        if (value)
        {
            Cells[index].Flags = static_cast<CellFlags>(Cells[index].Flags | CellFlags::RESERVED);
        }
        else
        {
            Cells[index].Flags = static_cast<CellFlags>(Cells[index].Flags & ~CellFlags::RESERVED);
        }
    }

    void SetAlreadyMoveAt(uint16_t index, bool value)
    {
        if (value)
        {
            Cells[index].Flags = static_cast<CellFlags>(Cells[index].Flags | CellFlags::HASMOVED);
        }
        else
        {
            Cells[index].Flags = static_cast<CellFlags>(Cells[index].Flags & ~CellFlags::HASMOVED);
        }
    }

    void SetActiveAt(uint16_t index, bool value)
    {
        if (value)
        {
            Cells[index].Flags = static_cast<CellFlags>(Cells[index].Flags | CellFlags::ACTIVE);
        }
        else
        {
            Cells[index].Flags = static_cast<CellFlags>(Cells[index].Flags & ~CellFlags::ACTIVE);
        }
    }

    void SetTypeAt(uint16_t index, CellTypes type)
    {
        if (index < CHUNK_EXT3)
        {
            Cells[index].Type = type;
        }
    }

    void CopyTo(CellBuffer& target) const
    {
        std::memcpy(target.Cells, Cells, CHUNK_EXT3 * sizeof(Cell));
    }

    void Clear()
    {
        std::memset(Cells, 0, sizeof(Cells));
    }
};
