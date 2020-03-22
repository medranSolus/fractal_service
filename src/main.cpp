#include "Complex.h"
#include "Surface.h"
#include "Window.h"
#include <mpi.h>
#include <CL/cl.hpp>
#include <iostream>
#include <limits>

Surface mandelbrot(const uint32_t size, const Window<uint32_t>& work_space, Window<double> fractal, const double escape_r, const uint16_t max_iteration);
void show_cl_info();

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //show_cl_info();
    const uint32_t screen_size = abs(atoi(argv[1]));
    Surface image = mandelbrot(screen_size, { 0, screen_size, 0, screen_size / 2 }, { -2.0, 1.0, -1.5, 1.5 }, 2.0, atoi(argv[2]));
    image.Append(mandelbrot(screen_size, { 0, screen_size, screen_size / 2, screen_size }, { -2.0, 1.0, -1.5, 1.5 }, 2.0, atoi(argv[2])));
    image.Save("bin/mandelbrot");
    // triple_mandelbrot: -1.5, 1.5, -1.5, 1.5
    // z * z * z + c
    // mandelbrot({ 0, atoi(argv[1]), 0, atoi(argv[1]) }, { -1.5, 1.5, -1.5, 1.5 }, atoi(argv[2]));
    
    MPI_Finalize();
    return 0;
}

Surface mandelbrot(const uint32_t size, const Window<uint32_t>& work_space, Window<double> fractal, const double escape_r, const uint16_t max_iteration)
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
                if (z.Abs2() <= escape_r * escape_r + std::numeric_limits<double>::epsilon())
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
                float t = (static_cast<float>(iteration + 2) - log(log(z.Abs())) / log(2)) / (max_iteration * 1.01f);
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

void show_cl_info()
{
    cl_uint platform_count = 0;
    clGetPlatformIDs(0, nullptr, &platform_count);
    cl_platform_id* ids = new cl_platform_id[platform_count];
    clGetPlatformIDs(platform_count, ids, nullptr);
    // Get platform info.
    for (cl_uint i = 0; i < platform_count; ++i)
    {
        char name[1024] = { '\0' };
        std::cout << "Platform:\t" << i << std::endl;
        clGetPlatformInfo(ids[i], CL_PLATFORM_NAME, 1024, &name, NULL);
        std::cout << "Name:\t\t" << name << std::endl;
        clGetPlatformInfo(ids[i], CL_PLATFORM_VENDOR, 1024, &name, NULL);
        std::cout << "Vendor:\t\t" << name << std::endl;
        std::cout << std::endl;
        // Get device count.
        cl_uint deviceNumber;
        clGetDeviceIDs(ids[i], CL_DEVICE_TYPE_GPU, 0, NULL, &deviceNumber);
        if (0 == deviceNumber)
        {
            std::cout << "No OpenCL devices found on platform " << i << "." << std::endl;
        }
        // Get device identifiers.
        cl_device_id* deviceIds = new cl_device_id[deviceNumber];
        clGetDeviceIDs(ids[i], CL_DEVICE_TYPE_GPU, deviceNumber, deviceIds, &deviceNumber);
        // Get device info.
        for (cl_uint i = 0; i < deviceNumber; ++i)
        {
            char name[1024] = { '\0' };
            std::cout << "Device:\t\t" << i << std::endl;
            clGetDeviceInfo(deviceIds[i], CL_DEVICE_NAME, 1024, &name, NULL);
            std::cout << "Name:\t\t" << name << std::endl;
            clGetDeviceInfo(deviceIds[i], CL_DEVICE_VENDOR, 1024, &name, NULL);
            std::cout << "Vendor:\t\t" << name << std::endl;
            clGetDeviceInfo(deviceIds[i], CL_DEVICE_VERSION, 1024, &name, NULL);
            std::cout << "Version:\t" << name << std::endl;
        }
        std::cout << std::endl;
        delete[] deviceIds;
    }
    delete[] ids;
}