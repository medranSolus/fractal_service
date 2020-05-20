#pragma once
#include "Color.h"
#include <string>
#include <vector>
#include <memory>

class Surface
{
    class Image
    {
        uint32_t width = 0;
        uint32_t height = 0;
        std::unique_ptr<Color[]> data = nullptr;

    public:
        constexpr Image() noexcept {}
        Image(uint32_t width, uint32_t height) noexcept;
        Image(uint32_t width, uint32_t height, Color default_color) noexcept;
        Image(const Image &) = delete;
        Image& operator=(const Image &) = delete;
        inline Image(Image && img) noexcept : width(img.width), height(img.height), data(std::move(img.data)) {}
        Image& operator=(Image && img) noexcept;
        
        constexpr uint32_t GetWidth() const noexcept { return width; }
        constexpr uint32_t GetHeight() const noexcept { return height; }
        inline Color* GetRow(uint32_t y) const noexcept { return &data[width * y]; }
        inline Color& GetPixel(uint32_t x, uint32_t y) noexcept { return data[width * y + x]; }
        inline const Color* GetData() const noexcept { return data.get(); }
        inline Color* GetData() noexcept { return data.get(); }
    };

    uint32_t height = 0;
    std::vector<Image> images;

public:
    inline Surface() noexcept {}
    Surface(uint32_t width, uint32_t height) noexcept;
    Surface(uint32_t width, uint32_t height, Color default_color) noexcept;
    Surface(const Surface &) = delete;
    Surface& operator=(const Surface &) = delete;
    inline Surface(Surface && surf) noexcept : height(surf.height), images(std::move(surf.images)) {}
    Surface& operator=(Surface && surf) noexcept;

    constexpr uint32_t GetHeight() const noexcept { return height; }
    inline uint32_t GetWidth() const noexcept { return images.at(0).GetWidth(); }
    inline uint32_t GetSize() const noexcept { return GetWidth() * height; }
    inline const Color* GetData(uint32_t i) const noexcept { return images.at(i).GetData(); }
    inline Color* GetData(uint32_t i) noexcept { return images.at(i).GetData(); }

    Color& GetPixel(uint32_t x, uint32_t y) noexcept;
    bool Append(Surface && surf) noexcept;

    bool Save(const std::string& filename) const;
};