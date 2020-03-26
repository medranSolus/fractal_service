#include "Fractal.h"
#include <limits>
#include <complex>
typedef std::complex<long double> Complex;

Surface Fractal::Mandelbrot(uint32_t width, uint32_t height, const Window<uint32_t>& work_space,
    const Window<long double>& fractal, long double escape_r, uint16_t max_iteration,
    uint16_t color_offset, const RGB<float>& color_scale, int32_t power) noexcept
{
    const long double ratio = static_cast<long double>(height) / width;
    const uint32_t x_offset = (height > width ? 0.0 : width * (ratio - 1.0) / 2.0);
    const uint32_t y_offset = (height < width ? 0.0 : (height - width) / 2);
    const long double x_scale = fractal.Width() / (width * (height > width ? 1.0 : ratio));
    const long double y_scale = (height < width ? 1.0 : ratio) * fractal.Height() / height;
    Surface image(work_space.Width(), work_space.Height());
	for (uint32_t y = work_space.min_y; y < work_space.max_y; ++y)
	{
		for (uint32_t x = work_space.min_x; x < work_space.max_x; ++x)
		{
            Complex c(x_scale * (x + x_offset) + fractal.min_x, y_scale * (y - y_offset) + fractal.min_y);
            bool outside = false;
			uint16_t iteration = 0;
            Complex z;
			while (iteration < max_iteration)
			{
                z = pow(z, power) + c; // Classic madelbrot = 2
                //z = pow(z.Abs(), 2.0) + c; // Burning ship
                if (abs(z) <= escape_r + std::numeric_limits<long double>::epsilon())
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
                float t = (static_cast<float>(iteration + color_offset) - log(log(abs(z))) / log(2)) / (max_iteration * 1.01f);
                if (t < 0.0f + std::numeric_limits<float>::epsilon())
                    t = 0.0f;
                const float g = t * t;
                const float r = g * t;
                const float b = -t + g * 3.0 + r * -3.0;
                t *= r;
				image.GetPixel(x, y - work_space.min_y) = Color(9.0f * (-t + r), 15.0f * (g - 2.0f * r + t), -8.5f * (b + t));
			}
		}
	}
    return image;
}