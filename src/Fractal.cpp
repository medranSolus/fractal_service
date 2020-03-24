#include "Fractal.h"
#include <limits>
#include <complex>
typedef std::complex<double> Complex;

Surface Fractal::Mandelbrot(uint32_t width, uint32_t height, const Window<uint32_t>& work_space,
    Window<double> fractal, double escape_r, uint16_t max_iteration, uint16_t color_offset) noexcept
{
    const double ratio = static_cast<double>(height) / width;
    const uint32_t x_offset = (height > width ? 0.0 : width * (ratio - 1.0) / 2.0);
    const uint32_t y_offset = (height < width ? 0.0 : (height - width) / 2);
    const double x_scale = fractal.width() / (width * (height > width ? 1.0 : ratio));
    const double y_scale = (height < width ? 1.0 : ratio) * fractal.height() / height;
    Surface image(work_space.width(), work_space.height());
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
                z = z * z + c; // Classic madelbrot
                //z = pow(z.Abs(), 2.0) + c; // Burning ship
                if (abs(z) <= escape_r + std::numeric_limits<double>::epsilon())
				    ++iteration;
                else
                {
                    outside = true;
                    break;
                }
			}
            if (outside)
            {
                z = z * z + c;
                z = z * z + c;
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