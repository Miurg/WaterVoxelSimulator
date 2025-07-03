#pragma once

#include <cstring>
#include "SimulationDefinitions/SimulationConst.h"
#include "ECellFlags.h"
#include "ECellTypes.h"

struct Cell
{
    ECellTypes Type;
    ECellFlags Flags;

    Cell() : Type(ECellTypes::AIR), Flags(ECellFlags::NONE) {}

    bool IsAir() const
    {
        return Type == ECellTypes::AIR;
    }

    bool IsReserved() const
    {
        return (Flags & ECellFlags::RESERVED) == ECellFlags::RESERVED;
    }

    bool IsActive() const
    {
        return (Flags & ECellFlags::ACTIVE) == ECellFlags::ACTIVE;
    }

    bool IsAlreadyMove() const
    {
        return (Flags & ECellFlags::HASMOVED) == ECellFlags::HASMOVED;
    }

    void SetReserved(bool value)
    {
        if (value)
            Flags = static_cast<ECellFlags>(Flags | ECellFlags::RESERVED);
        else
            Flags = static_cast<ECellFlags>(Flags & ~ECellFlags::RESERVED);
    }

    void SetAlreadyMove(bool value)
    {
        if (value)
            Flags = static_cast<ECellFlags>(Flags | ECellFlags::HASMOVED);
        else
            Flags = static_cast<ECellFlags>(Flags & ~ECellFlags::HASMOVED);
    }

    void SetActive(bool value)
    {
        if (value)
            Flags = static_cast<ECellFlags>(Flags | ECellFlags::ACTIVE);
        else
            Flags = static_cast<ECellFlags>(Flags & ~ECellFlags::ACTIVE);
    }

    void SetType(ECellTypes type)
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
        return Cells[index].Type == ECellTypes::AIR;
    }

    bool IsReservedAt(uint16_t index) const
    {
        return (Cells[index].Flags & ECellFlags::RESERVED) == ECellFlags::RESERVED;
    }

    bool IsActiveAt(uint16_t index) const
    {
        return (Cells[index].Flags & ECellFlags::ACTIVE) == ECellFlags::ACTIVE;
    }

    bool IsAlreadyMoveAt(uint16_t index) const
    {
        return (Cells[index].Flags & ECellFlags::HASMOVED) == ECellFlags::HASMOVED;
    }

    void SetReservedAt(uint16_t index, bool value)
    {
        if (value)
        {
            Cells[index].Flags = static_cast<ECellFlags>(Cells[index].Flags | ECellFlags::RESERVED);
        }
        else
        {
            Cells[index].Flags = static_cast<ECellFlags>(Cells[index].Flags & ~ECellFlags::RESERVED);
        }
    }

    void SetAlreadyMoveAt(uint16_t index, bool value)
    {
        if (value)
        {
            Cells[index].Flags = static_cast<ECellFlags>(Cells[index].Flags | ECellFlags::HASMOVED);
        }
        else
        {
            Cells[index].Flags = static_cast<ECellFlags>(Cells[index].Flags & ~ECellFlags::HASMOVED);
        }
    }

    void SetActiveAt(uint16_t index, bool value)
    {
        if (value)
        {
            Cells[index].Flags = static_cast<ECellFlags>(Cells[index].Flags | ECellFlags::ACTIVE);
        }
        else
        {
            Cells[index].Flags = static_cast<ECellFlags>(Cells[index].Flags & ~ECellFlags::ACTIVE);
        }
    }

    void SetTypeAt(uint16_t index, ECellTypes type)
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
