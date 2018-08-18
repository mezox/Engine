#pragma once

#include <ostream>

namespace Engine::Math
{
    template<typename T>
    class Vector2
    {
    public:
        constexpr Vector2() = default;
        
        constexpr Vector2(const T pX, const T pY)
            : x(pX)
            , y(pY)
        {}
        
        constexpr Vector2(const T v)
            : x(v)
            , y(v)
        {}
        
        friend std::ostream& operator<<(std::ostream& outstream, const Vector2& v)
        {
            outstream << "Vector2(" << v.x << ", " << v.y << ")";
            return outstream;
        }
        
    public:
        T x{ 0 };
        T y{ 0 };
    };
}
