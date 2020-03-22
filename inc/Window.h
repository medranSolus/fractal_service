#pragma once

template<typename T>
struct Window
{
    T min_x;
    T max_x;
    T min_y;
    T max_y;

    constexpr T width() const noexcept { return max_x - min_x; }
    constexpr T height() const noexcept { return max_y - min_y; }
};