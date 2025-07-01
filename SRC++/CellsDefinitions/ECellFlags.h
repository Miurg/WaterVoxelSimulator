#pragma once

#include <cstdint>

enum class ECellFlags : uint8_t 
{
    NONE = 0,
    RESERVED = 1 << 0,  //00000001
    ACTIVE = 1 << 1,    //00000010
    HASMOVED = 1 << 2   //00000100
};

constexpr ECellFlags operator|(ECellFlags a, ECellFlags b) 
{
    return static_cast<ECellFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

constexpr ECellFlags operator&(ECellFlags a, ECellFlags b) 
{
    return static_cast<ECellFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr ECellFlags operator~(ECellFlags a) 
{
    return static_cast<ECellFlags>(~static_cast<uint8_t>(a));
}

constexpr ECellFlags& operator|=(ECellFlags& a, ECellFlags b) 
{
    return a = a | b;
}

constexpr ECellFlags& operator&=(ECellFlags& a, ECellFlags b) 
{
    return a = a & b;
}