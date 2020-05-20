#include "Surface.h"
#include "Logger.h"
#include <png.h>
#include <algorithm>
#include <cstdio>
#include <sys/stat.h>

Surface::Image::Image(uint32_t width, uint32_t height) noexcept
    : width(width), height(height)
{
    data = std::make_unique<Color[]>(width * height);
}

Surface::Image::Image(uint32_t width, uint32_t height, Color default_color) noexcept
    : width(width), height(height)
{
    uint64_t size = width * height;
    data = std::make_unique<Color[]>(size);
    std::fill_n(data.get(), size, default_color);
}

Surface::Image& Surface::Image::operator=(Image && img) noexcept
{
    width = img.width;
    height = img.height;
    data = std::move(img.data);
    return *this;
}

Surface::Surface(uint32_t width, uint32_t height) noexcept : height(height)
{
    images.emplace_back(width, height);
}

Surface::Surface(uint32_t width, uint32_t height, Color default_color) noexcept : height(height)
{
    images.emplace_back(width, height, default_color);
}

Surface& Surface::operator=(Surface && surf) noexcept
{
    height = surf.height;
    images = std::move(surf.images);
    return *this;
}

Color& Surface::GetPixel(uint32_t x, uint32_t y) noexcept
{
    for (auto& image : images)
    {
        if (y < image.GetHeight())
            return image.GetPixel(x, y);
        else
            y -= image.GetHeight();
    }
    return images.at(0).GetPixel(0, 0);
}

bool Surface::Append(Surface && surf) noexcept
{
    if (surf.GetWidth() != GetWidth())
    {
        Logger::LogError("Trying to concatenate surface with not matching width! ("
            + std::to_string(surf.GetWidth()) + ") Expected: " + std::to_string(GetWidth()));
        return false;
    }
    height += surf.height;
    images.reserve(height);
    images.insert(images.end(), std::make_move_iterator(surf.images.begin()),
        std::make_move_iterator(surf.images.end()));
    return true;
}

bool Surface::Save(const std::string& filename) const
{
    FILE* file = fopen((filename + ".png").c_str(), "wb");
    if (file == NULL)
    {
        Logger::LogError("Cannot create file: " + filename);
        return false;
    }
	png_structp png_write_header = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (png_write_header == NULL)
    {
        Logger::LogError("Cannot create PNG write structure for file: " + filename);
        fclose(file);
        return false;
	}

	png_infop png_info_header = png_create_info_struct(png_write_header);
	if (png_info_header == NULL)
    {
        Logger::LogError("Cannot create PNG info structure for file: " + filename);
        fclose(file);
        png_destroy_write_struct(&png_write_header, nullptr);
        return false;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_write_header)))
    {
        Logger::LogError("Error saving to PNG file: " + filename);
		fclose(file);
        png_destroy_write_struct(&png_write_header, nullptr);
        png_free_data(png_write_header, png_info_header, PNG_FREE_ALL, -1);
		return false;
	}
	png_init_io(png_write_header, file);
    
	// Write header (24 bit colour depth)
	png_set_IHDR(png_write_header, png_info_header, GetWidth(), height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_ADAM7,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_write_header, png_info_header);
    png_set_packing(png_write_header);

	// Write image data
    png_byte** rows = new png_byte*[height];
    for (uint32_t y = 0, i = 0, i_y = 0; y < height; ++y, ++i_y)
    {
        if (i_y >= images.at(i).GetHeight())
        {
            ++i;
            i_y = 0;
        }
        rows[y] = reinterpret_cast<png_byte*>(images.at(i).GetRow(i_y));
    }
        
    png_write_image(png_write_header, rows);
    png_write_end(png_write_header, png_info_header);
    
    fclose(file);
    png_free_data(png_write_header, png_info_header, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_write_header, nullptr);
    delete[] rows;

    chmod(filename.c_str(), 0777);
	return true;
}