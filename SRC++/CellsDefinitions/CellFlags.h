#pragma once

#include <cstdint>

enum class CellFlags : uint8_t 
{
    NONE = 0,
    RESERVED = 1 << 0,  //00000001
    ACTIVE = 1 << 1,    //00000010
    HASMOVED = 1 << 2   //00000100
};

//Overloading operators
constexpr CellFlags operator|(CellFlags a, CellFlags b) 
{
    return static_cast<CellFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

constexpr CellFlags operator&(CellFlags a, CellFlags b) 
{
    return static_cast<CellFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr CellFlags operator~(CellFlags a) 
{
    return static_cast<CellFlags>(~static_cast<uint8_t>(a));
}

constexpr CellFlags& operator|=(CellFlags& a, CellFlags b) 
{
    return a = a | b;
}

constexpr CellFlags& operator&=(CellFlags& a, CellFlags b) 
{
    return a = a & b;
}