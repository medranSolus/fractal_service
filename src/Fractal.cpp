#include "Fractal.h"
#include <limits>
#include <complex>
typedef std::complex<double> Complex;

Surface Fractal::Mandelbrot(const uint32_t size, const Window<uint32_t>& work_space,
    Window<double> fractal, const double escape_r, const uint16_t max_iteration) noexcept
{
    const double fractal_width = fractal.width();
    const double fractal_height = fractal.height();
    Surface image(work_space.width(), work_space.height());
	for (uint32_t y = work_space.min_y; y < work_space.max_y; ++y)
	{
		for (uint32_t x = work_space.min_x; x < work_space.max_x; ++x)
		{
            Complex c((fractal_width * x) / size + fractal.min_x, (fractal_height * y) / size + fractal.min_y);
            bool outside = false;
			uint16_t iteration = 0;
            Complex z;
			while (iteration < max_iteration)
			{
                z = (z * z) + c; // Classic madelbrot
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
                z = (z * z) + c;
                z = (z * z) + c;
                float t = (static_cast<float>(iteration + 2) - log(log(abs(z))) / log(2)) / (max_iteration * 1.01f);
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