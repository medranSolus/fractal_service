#pragma once
#include "Color.h"
#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

class Surface
{
    class Image
    {
        uint32_t width;
        uint32_t height;
        std::unique_ptr<Color[]> data = nullptr;

    public:
        Image(uint32_t width, uint32_t height, Color default_color = Color{}) noexcept;
        Image(const Image &) = delete;
        Image& operator=(const Image &) = delete;
        inline Image(Image && img) noexcept : width(img.width), height(img.height), data(std::move(img.data)) {}
        Image& operator=(Image && img) noexcept;
        
        constexpr uint32_t GetWidth() const noexcept { return width; }
        constexpr uint32_t GetHeight() const noexcept { return height; }
        inline Color* GetRow(uint32_t y) const noexcept { return &data[width * y]; }
        inline Color& GetPixel(uint32_t x, uint32_t y) noexcept { return data[width * y + x]; }
        inline const Color* GetData() const noexcept { return data.get(); }
    };

    uint32_t height;
    std::vector<Image> images;

public:
    Surface(uint32_t width, uint32_t height, Color default_color = Color{}) noexcept;
    Surface(const Surface &) = delete;
    Surface& operator=(const Surface &) = delete;
    inline Surface(Surface && surf) noexcept : height(surf.height), images(std::move(surf.images)) {}
    Surface& operator=(Surface && surf) noexcept;

    constexpr uint32_t GetHeight() const noexcept { return height; }
    inline uint32_t GetWidth() const noexcept { return images.at(0).GetWidth(); }
    inline const Color* GetData(uint32_t i) const noexcept { return images.at(i).GetData(); }

    Color& GetPixel(uint32_t x, uint32_t y) noexcept;
    bool Append(Surface && surf) noexcept;

    bool Save(const std::string& filename) const;
};