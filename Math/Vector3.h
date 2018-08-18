#pragma once

#include "Math/Vector2.h"

#include <ostream>

namespace Engine::Math
{
    template<typename T>
    class Vector3
    {
    public:
        constexpr Vector3() = default;
        
        constexpr Vector3(const T pX, const T pY, const T pZ) : x(pX), y(pY), z(pZ) {}
        constexpr Vector3(const T v) : x(v), y(v), z(v) {}
        constexpr Vector3(const T pX, const Vector2<T>& yz): x(pX), y(yz.x), z(yz.y) {}
        constexpr Vector3(const Vector2<T>& xy, const T pZ): x(xy.x), y(xy.y), z(pZ) {}
        
        friend std::ostream& operator<<(std::ostream& outstream, const Vector3& v)
        {
            outstream << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
            return outstream;
        }
        
    public:
        T x{ 0 };
        T y{ 0 };
        T z{ 0 };
    };
}
