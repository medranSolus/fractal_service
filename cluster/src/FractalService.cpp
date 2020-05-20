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

Surface FractalService::Mandelbrot(Net::JobData& data, uint32_t min_y, uint32_t max_y, bool color_iterative) noexcept
{
    const FractalSpace fractal = mandelbrot_space.Move(data.offset_x * data.zoom, -data.offset_y * data.zoom) / data.zoom;
    const double ratio = static_cast<double>(data.height) / data.width;
    const bool wide_screen = data.height < data.width;
    const uint32_t work_height = max_y - min_y;
    Surface image(data.width, work_height);
    MandelbrotParams params =
    {
        data.escape_r + std::numeric_limits<double>::epsilon(),
        fractal.min_x,
        fractal.min_y,
        fractal.Width() / (data.width * (!wide_screen ? 1.0 : ratio)),
        (wide_screen ? 1.0 : ratio) * fractal.Height() / data.height,
        static_cast<cl_uint>(!wide_screen ? 0.0 : data.width * (ratio - 1.0) / 2),
        static_cast<cl_uint>(wide_screen ? 0.0 : (data.height - data.width) / 2),
        min_y,
        data.power,
        data.iterations,
        data.color_offset,
        data.channels,
        static_cast<cl_uchar>(color_iterative ? 1 : 0)
    };
    const size_t byte_size = sizeof(Color) * data.width * work_height;
    cl::Buffer pixel_buffer(cl_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, byte_size, image.GetData(0));
    cl::Buffer params_buffer(cl_context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(params), &params);
    cl::Kernel kernel(mandelbrot_program, "mandelbrot");
    kernel.setArg(0, pixel_buffer);
    kernel.setArg(1, params_buffer);
    cl::CommandQueue queue(cl_context, cl_device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(data.width, work_height));
    queue.enqueueReadBuffer(pixel_buffer, CL_TRUE, 0, byte_size, image.GetData(0));
    return image;
}

Surface FractalService::Mandelbrot_OLD(Net::JobData& data, uint32_t min_y, uint32_t max_y) noexcept
{
    const FractalSpace fractal = mandelbrot_space.Move(data.offset_x * data.zoom, -data.offset_y * data.zoom) / data.zoom;
    const double ratio = static_cast<double>(data.height) / data.width;
    const bool wide_screen = data.height < data.width;
    const double x_scale = fractal.Width() / (data.width * (!wide_screen ? 1.0 : ratio));
    const double y_scale = (wide_screen ? 1.0 : ratio) * fractal.Height() / data.height;
    const uint32_t x_offset = (!wide_screen ? 0.0 : data.width * (ratio - 1.0) / 2.0);
    const uint32_t y_offset = (wide_screen ? 0.0 : (data.height - data.width) / 2);
    data.escape_r += std::numeric_limits<double>::epsilon();
    Surface image(data.width, max_y - min_y);
	for (uint32_t y = min_y; y < max_y; ++y)
	{
		for (uint32_t x = 0; x < data.width; ++x)
		{
            Complex c(x_scale * (x + x_offset) + fractal.min_x, y_scale * (y - y_offset) + fractal.min_y);
            bool outside = false;
			uint16_t iteration = 0;
            Complex z;
			while (iteration < data.iterations)
			{
                z = pow(z, data.power) + c; // Classic madelbrot = 2
                if (abs(z) <= data.escape_r)
				    ++iteration;
                else
                {
                    outside = true;
                    break;
                }
			}
            if (outside)
            {
                z = pow(z, data.power) + c;
                z = pow(z, data.power) + c;
                float t = (static_cast<float>(iteration + data.color_offset + 2) - log(log(abs(z))) / log(2)) / (data.iterations * 1.01f);
                if (t >= std::numeric_limits<float>::epsilon())
                {
                    const float g = t * t;
                    const float r = g * t;
                    const float b = -t + g * 3.0f + r * -3.0f;
                    t *= r;
                    image.GetPixel(x, y - min_y) = Color(data.channels.r * (-t + r), data.channels.g * (g - 2.0f * r + t), -data.channels.b * (b + t));
                }
			}
		}
	}
    return image;
}