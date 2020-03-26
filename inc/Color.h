#pragma once
#include <stdint.h>

template<typename T>
struct RGB
{
    T r = static_cast<T>(0);
    T g = static_cast<T>(0);
    T b = static_cast<T>(0);
};


class Color
{
    RGB<uint8_t> channels;

public:
    Color() = default;
    constexpr Color(float r, float g, float b) noexcept
        : channels({static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255)}) {}
    Color(const Color&) = default;
    Color(Color&&) = default;
    Color& operator=(const Color&) = default;
    Color& operator=(Color&&) = default;
    ~Color() = default;

    constexpr uint8_t GetR() const noexcept { return channels.r; }
    constexpr uint8_t GetG() const noexcept { return channels.g; }
    constexpr uint8_t GetB() const noexcept { return channels.b; }
};