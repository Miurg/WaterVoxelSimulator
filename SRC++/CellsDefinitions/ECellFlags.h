#pragma once

#include <cstdint>

enum class ECellFlags : uint8_t
{
    NONE = 0,
    RESERVED = 1 << 0,  //00000001
    ACTIVE = 1 << 1,    //00000010
    HASMOVED = 1 << 2   //00000100
    //Direction stores in (11111000)
};

constexpr uint8_t DIRECTION_MASK = 0b11111000; 
constexpr uint8_t DIRECTION_SHIFT = 3;

enum class EDirection : uint8_t
{
    RIGHT,
    LEFT,
    FORWARD,
    BACKWARD,
    UP,
    DOWN,

    UP_RIGHT,
    UP_LEFT,
    DOWN_RIGHT,
    DOWN_LEFT,
    FORWARD_RIGHT,
    FORWARD_LEFT,
    BACKWARD_RIGHT,
    BACKWARD_LEFT,
    UP_FORWARD,
    UP_BACKWARD,
    DOWN_FORWARD,
    DOWN_BACKWARD
    // 6 + 12 = 18
};

inline void SetDirection(ECellFlags& flag, EDirection dir)
{
    uint8_t tmp = static_cast<uint8_t>(flag);
    tmp &= ~DIRECTION_MASK;
    tmp |= (static_cast<uint8_t>(dir) << DIRECTION_SHIFT) & DIRECTION_MASK;
    flag = static_cast<ECellFlags>(tmp);
}

inline EDirection GetDirection(ECellFlags& flag)
{
    uint8_t tmp = static_cast<uint8_t>(flag);
    return static_cast<EDirection>((tmp & DIRECTION_MASK) >> DIRECTION_SHIFT);
}

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