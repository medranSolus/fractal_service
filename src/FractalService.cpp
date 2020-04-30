#include "FractalService.h"
#include "Logger.h"
#include <limits>
#include <fstream>
#include <complex>
#include <stdexcept>
typedef std::complex<double> Complex;

std::string FractalService::LoadProgram(const std::string& filename)
{
    std::ifstream fin("opencl/" + filename);
    if (!fin.is_open())
    {
        const std::string error_msg = "Cannot open program \"opencl/" + filename + "\"!";
        Logger::LogError(error_msg);
        throw std::runtime_error(error_msg);
    }
    std::string code(std::istreambuf_iterator<char>(fin), (std::istreambuf_iterator<char>()));
    fin.close();
    return code;
}

Surface FractalService::Mandelbrot(uint32_t width, uint32_t height, uint32_t min_y, uint32_t max_y,
    double offset_x, double offset_y, double zoom, double escape_r, uint16_t max_iteration,
    uint16_t color_offset, const RGB<float>& color_scale, int32_t power, bool color_iterative) noexcept
{
    const FractalSpace fractal = mandelbrot_space.Move(offset_x * zoom, -offset_y * zoom) / zoom;
    const double ratio = static_cast<double>(height) / width;
    const bool wide_screen = height < width;
    const uint32_t work_height = max_y - min_y;
    Surface image(width, work_height);
    MandelbrotParams params =
    {
        escape_r + std::numeric_limits<double>::epsilon(),
        fractal.min_x,
        fractal.min_y,
        fractal.Width() / (width * (!wide_screen ? 1.0 : ratio)),
        (wide_screen ? 1.0 : ratio) * fractal.Height() / height,
        static_cast<cl_uint>(!wide_screen ? 0.0 : width * (ratio - 1.0) / 2),
        static_cast<cl_uint>(wide_screen ? 0.0 : (height - width) / 2),
        min_y,
        power,
        max_iteration,
        color_offset,
        color_scale,
        static_cast<cl_uchar>(color_iterative ? 1 : 0)
    };
    const size_t byte_size = sizeof(Color) * width * work_height;
    cl::Buffer pixel_buffer(cl_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, byte_size, image.GetData(0));
    cl::Buffer params_buffer(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(params), &params);
    cl::Kernel kernel(mandelbrot_program, "mandelbrot");
    kernel.setArg(0, pixel_buffer);
    kernel.setArg(1, params_buffer);
    cl::CommandQueue queue(cl_context, cl_device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(width, work_height));
    queue.enqueueReadBuffer(pixel_buffer, CL_TRUE, 0, byte_size, image.GetData(0));
    return image;
}

Surface FractalService::Mandelbrot_OLD(uint32_t width, uint32_t height, uint32_t min_y, uint32_t max_y,
    double offset_x, double offset_y, double zoom, double escape_r, uint16_t max_iteration,
    uint16_t color_offset, const RGB<float>& color_scale, int32_t power) noexcept
{
    const FractalSpace fractal = mandelbrot_space.Move(offset_x * zoom, -offset_y * zoom) / zoom;
    const double ratio = static_cast<double>(height) / width;
    const bool wide_screen = height < width;
    const double x_scale = fractal.Width() / (width * (!wide_screen ? 1.0 : ratio));
    const double y_scale = (wide_screen ? 1.0 : ratio) * fractal.Height() / height;
    const uint32_t x_offset = (!wide_screen ? 0.0 : width * (ratio - 1.0) / 2.0);
    const uint32_t y_offset = (wide_screen ? 0.0 : (height - width) / 2);
    escape_r += std::numeric_limits<double>::epsilon();
    Surface image(width, max_y - min_y);
	for (uint32_t y = min_y; y < max_y; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
            Complex c(x_scale * (x + x_offset) + fractal.min_x, y_scale * (y - y_offset) + fractal.min_y);
            bool outside = false;
			uint16_t iteration = 0;
            Complex z;
			while (iteration < max_iteration)
			{
                z = pow(z, power) + c; // Classic madelbrot = 2
                if (abs(z) <= escape_r)
				    ++iteration;
                else
                {
                    outside = true;
                    break;
                }
			}
            if (outside)
            {
                z = pow(z, power) + c;
                z = pow(z, power) + c;
                float t = (static_cast<float>(iteration + color_offset + 2) - log(log(abs(z))) / log(2)) / (max_iteration * 1.01f);
                if (t >= std::numeric_limits<float>::epsilon())
                {
                    const float g = t * t;
                    const float r = g * t;
                    const float b = -t + g * 3.0f + r * -3.0f;
                    t *= r;
                    image.GetPixel(x, y - min_y) = Color(color_scale.r * (-t + r), color_scale.g * (g - 2.0f * r + t), -color_scale.b * (b + t));
                }
			}
		}
	}
    return image;
}