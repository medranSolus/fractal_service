#pragma once
#include "Surface.h"
#include "Window.h"
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class FractalService
{
    #pragma pack(push, 1)
    struct MandelbrotParams
    {
        cl_double escape_r;
        cl_double frac_min_x;
        cl_double frac_min_y;
        cl_double x_scale;
        cl_double y_scale;
        cl_uint x_offset;
        cl_uint y_offset;
        cl_uint min_y;
        cl_int power;
        cl_ushort max_iteration;
        cl_ushort color_offset;
        RGB<float> color_scale;
        cl_uchar color_iterative;
    };
    #pragma pack(pop)

    typedef Window<double> FractalSpace;
    static constexpr FractalSpace mandelbrot_space = { -2.0, 1.0, -1.5, 1.5 };
    static constexpr FractalSpace triplebrot_space = { -1.5, 1.5, -1.5, 1.5 };

    cl::Device cl_device;
    cl::Context cl_context;
    cl::Program mandelbrot_program;

    static std::string LoadProgram(const std::string& filename);

public:
    inline FractalService(const cl::Device& device)
        : cl_device(device), cl_context(cl_device), mandelbrot_program(cl_context, LoadProgram("mandelbrot.cl"), true) {}
    
    Surface Mandelbrot(uint32_t width, uint32_t height, uint32_t min_y, uint32_t max_y,
        double offset_x, double offset_y, double zoom, double escape_r, uint16_t max_iteration,
        uint16_t color_offset, const RGB<float>& color_scale, int32_t power, bool color_iterative) noexcept;
    Surface Mandelbrot_OLD(uint32_t width, uint32_t height, uint32_t min_y, uint32_t max_y,
        double offset_x, double offset_y, double zoom, double escape_r, uint16_t max_iteration,
        uint16_t color_offset, const RGB<float>& color_scale, int32_t power) noexcept;
};

// Newton: z-=a*((z*z*z - 1.0) / (3.0*z*z)); // a = any number
// z = pow(abs(z), 2.0) + c; // Burning ship
