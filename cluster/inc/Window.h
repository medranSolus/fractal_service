#pragma once

template<typename T>
struct Window
{
    T min_x;
    T max_x;
    T min_y;
    T max_y;

    constexpr T Width() const noexcept { return max_x - min_x; }
    constexpr T Height() const noexcept { return max_y - min_y; }
    constexpr Window Move(T x, T y) const noexcept { return { min_x + x, max_x + x, min_y + y, max_y + y }; }

    constexpr Window operator*(T x) const noexcept { return { min_x * x, max_x * x, min_y * x, max_y * x }; }
    constexpr Window operator*=(T x) noexcept { *this = *this * x; return *this; }
    constexpr Window operator/(T x) const noexcept { return { min_x / x, max_x / x, min_y / x, max_y / x }; }
    constexpr Window operator/=(T x) noexcept { *this = *this / x; return *this; }
};