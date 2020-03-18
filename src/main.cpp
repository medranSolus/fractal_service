#include "Complex.h"
#include <mpi.h>
#include <CL/cl.hpp>
#include <iostream>

void show_cl_info();

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    show_cl_info();
    size_t width = 800, height = 600;

    constexpr cl_float min_x = -2.0f, max_x = 1.0f;
    constexpr cl_float min_y = -1.0f, max_y = 1.0f;
    const cl_float step_x = (max_x - min_x) / width;
    const cl_float step_y = (max_y - min_y) / height;
    
    MPI_Finalize();
    return 0;
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