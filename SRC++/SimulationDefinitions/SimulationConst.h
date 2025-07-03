#pragma once

#include <cstdint>
#include <vector>
#include <array>

//=== Chunk Size Configuration ===
static constexpr uint_fast16_t  CHUNK_SIZE = 32;
static constexpr uint_fast16_t  CHUNK_SIZE2 = CHUNK_SIZE * CHUNK_SIZE;
static constexpr uint_fast16_t  CHUNK_SIZE3 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

//Extended chunk size (with dead cell layers)
static constexpr uint_fast16_t  CHUNK_EXT = CHUNK_SIZE + 2; //One layer from each side for dead cells
static constexpr uint_fast16_t  CHUNK_EXT2 = CHUNK_EXT * CHUNK_EXT;
static constexpr uint_fast16_t  CHUNK_EXT3 = CHUNK_EXT * CHUNK_EXT * CHUNK_EXT;

//=== Bit manipulation helpers ===
static constexpr uint8_t SIZE_LOG2 = []() 
    {
    uint8_t log = 0;
    uint8_t val = CHUNK_SIZE;
    while (val >>= 1) ++log;
    return log;
    }();

//Bit shifts for coordinate calculations (only CHUNK_SIZE)
static constexpr uint8_t XY_SHIFT = SIZE_LOG2;
static constexpr uint8_t Z_SHIFT = 2 * SIZE_LOG2;

//=== Dead and Border Layer Detection ===
extern bool isDeadLayerCell[CHUNK_EXT3];
extern bool isBorderLayerCell[CHUNK_EXT3];

inline bool IsBorderLayerCell(uint_fast16_t index)
{
    return isBorderLayerCell[index] != 0;
}

inline bool IsDeadLayerCell(uint_fast16_t index)
{
    return isDeadLayerCell[index] != 0;
}

//=== Initialization Classes ===
struct BorderLayerMaskInitializer
{
    BorderLayerMaskInitializer();
};

struct DeadLayerMaskInitializer 
{
    DeadLayerMaskInitializer();
};

struct BondaryIndicesInitializer
{
    BondaryIndicesInitializer();
};

//=== Boundary Management ===
struct BoundaryIndices
{
    std::vector<uint_fast16_t> deadCells;
    std::vector<uint_fast16_t> boundaryCells;
};
extern std::array<BoundaryIndices, 27> boundaryPairs;

//Global initializers
extern BorderLayerMaskInitializer borderLayerMaskInitializer;
extern DeadLayerMaskInitializer deadLayerMaskInitializer;
extern BondaryIndicesInitializer bondaryIndicesInitializer;