#pragma once
#include "Surface.h"
#include "Window.h"

class Fractal
{
public:
    Fractal() = delete;
    
    static Surface Mandelbrot(uint32_t width, uint32_t height, const Window<uint32_t>& work_space,
        const Window<long double>& fractal, long double escape_r, uint16_t max_iteration,
        uint16_t color_offset, const RGB<float>& color_scale, int32_t power) noexcept;
};

// triple_mandelbrot: -1.5, 1.5, -1.5, 1.5
// z * z * z + c
// mandelbrot({ 0, atoi(argv[1]), 0, atoi(argv[1]) }, { -1.5, 1.5, -1.5, 1.5 }, atoi(argv[2]));
// Newton: z-=a*((z*z*z - 1.0) / (3.0*z*z)); // a = any complex number
