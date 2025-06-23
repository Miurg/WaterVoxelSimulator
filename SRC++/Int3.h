#pragma once

#include <godot_cpp/core/math.hpp>
#include <functional> 
#include <string>

using namespace godot;

struct int3 
{
    int x;
    int y;
    int z;

    int3() : x(0), y(0), z(0) {}
    int3(int x, int y, int z) : x(x), y(y), z(z) {}

    int3(const Vector3& v) :
        x(static_cast<int>(Math::floor(v.x))),
        y(static_cast<int>(Math::floor(v.y))),
        z(static_cast<int>(Math::floor(v.z))) {}

    Vector3 ToVector3() const 
    {
        return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    }

    bool operator==(const int3& other) const 
    {
        return x == other.x && y == other.y && z == other.z;
    }

    int3 operator+(const int3& other) const 
    {
        return int3(x + other.x, y + other.y, z + other.z);
    }

    int3 operator-(const int3& other) const 
    {
        return int3(x - other.x, y - other.y, z - other.z);
    }

    std::string ToString() const 
    {
        return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
    }
}; 
namespace std 
{
    template <>
    struct hash<int3> 
    {
        std::size_t operator()(const int3& k) const 
        {
            std::size_t hx = std::hash<int>()(k.x);
            std::size_t hy = std::hash<int>()(k.y);
            std::size_t hz = std::hash<int>()(k.z);

            return ((hx ^ (hy << 1)) >> 1) ^ (hz << 1);
        }
    };
}