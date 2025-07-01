#pragma once
#include <cstdint>
#include "SimulationConst.h"
#include <godot_cpp/variant/vector3i.hpp>

inline int ChunkDirectionToIndex(int dx, int dy, int dz)
{
    return (dx + 1) * 9 + (dy + 1) * 3 + (dz + 1); // [0..26]
}

inline int InverseForChunkDirection(int i)
{
    int dx = i / 9 - 1;
    int dy = (i / 3) % 3 - 1;
    int dz = i % 3 - 1;
    return (1 - dx) * 9 + (1 - dy) * 3 + (1 - dz);
}

inline std::tuple<int, int, int> ChunkIndexToDirection(int index) 
{
    int dx = index / 9 - 1;
    int dy = (index / 3) % 3 - 1;
    int dz = index % 3 - 1;
    return { dx, dy, dz };
}

constexpr inline uint_fast16_t SIZECellVectorToIndex(const godot::Vector3i& pos)
{
    return pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE2;
}

inline godot::Vector3i SIZECellIndexToVector(uint_fast16_t index)
{
    int z = index / CHUNK_SIZE2;
    int y = (index / CHUNK_SIZE) % CHUNK_SIZE;
    int x = index % CHUNK_SIZE;
    return godot::Vector3i(x, y, z);
}

inline godot::Vector3i SIZECellFastIndexToVector(uint_fast16_t index)
{
    int z = index >> (2 * SIZE_LOG2);           // index / CHUNK_SIZE?
    int y = (index >> SIZE_LOG2) & (CHUNK_SIZE - 1);  // (index / CHUNK_SIZE) % CHUNK_SIZE
    int x = index & (CHUNK_SIZE - 1);           // index % CHUNK_SIZE
    return godot::Vector3i(x, y, z);
}

constexpr inline uint_fast32_t SIZECellFastToIndex(uint_fast16_t x, uint_fast16_t y, uint_fast16_t z) 
{
    return x + (y << XY_SHIFT) + (z << Z_SHIFT);
}

constexpr inline uint_fast16_t EXTCellVectorToIndex(const godot::Vector3i& pos)
{
    return pos.x + pos.y * CHUNK_EXT + pos.z * CHUNK_EXT2;
}

inline godot::Vector3i EXTCellIndexToVector(uint_fast32_t index) 
{
    const uint_fast32_t z = index / CHUNK_EXT2;
    const uint_fast32_t remainder = index % CHUNK_EXT2;
    const uint_fast32_t y = remainder / CHUNK_EXT;
    const uint_fast32_t x = remainder % CHUNK_EXT;
    return godot::Vector3i(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
}

inline int FloorDiv(int a, int b)
{
    int div = a / b;
    int rem = a % b;
    if ((rem != 0) && ((b < 0) != (rem < 0)))
    {
        div--;
    }
    return div;
}
