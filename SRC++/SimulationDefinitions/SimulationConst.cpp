#include "SimulationConst.h"
#include <godot_cpp/variant/vector3i.hpp>
#include <array>
#include "SimulationUtils.h"

bool isBorderLayerCell[CHUNK_EXT3];

BorderLayerMaskInitializer::BorderLayerMaskInitializer()
{
    for (uint_fast16_t z = 0; z < CHUNK_EXT; ++z)
    {
        uint_fast16_t zOffset = z * CHUNK_EXT * CHUNK_EXT;
        for (uint_fast16_t y = 0; y < CHUNK_EXT; ++y)
        {
            uint_fast16_t yOffset = y * CHUNK_EXT;
            for (uint_fast16_t x = 0; x < CHUNK_EXT; ++x)
            {
                uint_fast16_t index = x + yOffset + zOffset;
                isBorderLayerCell[index] = (x == 1 || x == CHUNK_EXT - 2) ||
                    (y == 1 || y == CHUNK_EXT - 2) ||
                    (z == 1 || z == CHUNK_EXT - 2);
            }
        }
    }
}

BorderLayerMaskInitializer borderLayerMaskInitializer;

bool isDeadLayerCell[CHUNK_EXT3];

DeadLayerMaskInitializer::DeadLayerMaskInitializer() 
{
    for (uint_fast16_t z = 0; z < CHUNK_EXT; ++z) 
    {
        uint_fast16_t zOffset = z * CHUNK_EXT * CHUNK_EXT;
        for (uint_fast16_t y = 0; y < CHUNK_EXT; ++y) 
        {
            uint_fast16_t yOffset = y * CHUNK_EXT;
            for (uint_fast16_t x = 0; x < CHUNK_EXT; ++x) 
            {
                uint_fast16_t index = x + yOffset + zOffset;
                isDeadLayerCell[index] = (x == 0 || x == CHUNK_EXT - 1) ||
                    (y == 0 || y == CHUNK_EXT - 1) ||
                    (z == 0 || z == CHUNK_EXT - 1);
            }
        }
    }
}

DeadLayerMaskInitializer deadLayerMaskInitializer;

std::array<BoundaryIndices, 27> boundaryPairs;

godot::Vector3i GetNeighborBorderCoord(int dx, int dy, int dz, int x, int y, int z)
{
    int nx = x;
    int ny = y;
    int nz = z;

    if (dx == -1)
    {
        nx = CHUNK_EXT - 2;
    }
    else if (dx == 1)
    {
        nx = 1;
    }

    if (dy == -1)
    {
        ny = CHUNK_EXT - 2;
    }
    else if (dy == 1)
    {
        ny = 1;
    }

    if (dz == -1)
    {
        nz = CHUNK_EXT - 2;
    }
    else if (dz == 1)
    {
        nz = 1;
    }

    return godot::Vector3i(nx, ny, nz);
}
void ComputeAndStoreBoundaryIndices(int dx, int dy, int dz, BoundaryIndices& boundaryPair)
{
    //-1 - left side(start with 0, end with 0), 0 - center(start with 1, end with CHUNK_SIZE), 1 - right side (start with CHUNK_EXT-1, end with CHUNK_EXT-1; 
    int xStart = (dx == -1) ? 0 : (dx == 1) ? CHUNK_EXT - 1 : 1;
    int xEnd = (dx == -1) ? 0 : (dx == 1) ? CHUNK_EXT - 1 : CHUNK_EXT - 2;

    int yStart = (dy == -1) ? 0 : (dy == 1) ? CHUNK_EXT - 1 : 1;
    int yEnd = (dy == -1) ? 0 : (dy == 1) ? CHUNK_EXT - 1 : CHUNK_EXT - 2;

    int zStart = (dz == -1) ? 0 : (dz == 1) ? CHUNK_EXT - 1 : 1;
    int zEnd = (dz == -1) ? 0 : (dz == 1) ? CHUNK_EXT - 1 : CHUNK_EXT - 2;
    for (int x = xStart; x <= xEnd; ++x)
    {
        for (int y = yStart; y <= yEnd; ++y)
        {
            for (int z = zStart; z <= zEnd; ++z)
            {

                godot::Vector3i neighborCoord = GetNeighborBorderCoord(dx, dy, dz, x, y, z);

                boundaryPair.deadCells.push_back(EXTCellVectorToIndex(godot::Vector3i(x, y, z)));
                boundaryPair.boundaryCells.push_back((EXTCellVectorToIndex(neighborCoord)));
            }
        }
    }
}
BondaryIndicesInitializer::BondaryIndicesInitializer()
{
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dz = -1; dz <= 1; ++dz)
            {
                if (dx == 0 && dy == 0 && dz == 0) continue;
                int index = ChunkDirectionToIndex(dx, dy, dz);

                ComputeAndStoreBoundaryIndices(dx, dy, dz, boundaryPairs[index]);
            }
        }
    }
}

BondaryIndicesInitializer bondaryIndicesInitializer;