#pragma once
#include <stdint.h>

class Color
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

public:
    Color() = default;
    constexpr Color(float r, float g, float b) noexcept
        : r(static_cast<uint8_t>(r * 255)), g(static_cast<uint8_t>(g * 255)), b(static_cast<uint8_t>(b * 255)) {}
    Color(const Color&) = default;
    Color(Color&&) = default;
    Color& operator=(const Color&) = default;
    Color& operator=(Color&&) = default;
    ~Color() = default;

    constexpr uint8_t GetR() const noexcept { return r; }
    constexpr uint8_t GetG() const noexcept { return g; }
    constexpr uint8_t GetB() const noexcept { return b; }
};