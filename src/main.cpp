#include "MPI/Master.h"
#include "MPI/Slave.h"
#include <CL/cl.hpp>
#include <iostream>

void show_cl_info();

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //show_cl_info();
    if (rank == 0)
    {
        int stations;
        MPI_Comm_size(MPI_COMM_WORLD, &stations);
        rank = MPI::Master(stations - 1).Run();

    }
    else
        rank = MPI::Slave(rank).Run();
    MPI_Finalize();
    return rank;
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