#include "MPI/Master.h"
#include "MPI/Slave.h"
#include <CL/cl.hpp>
#include <iostream>
#include "MPI/Messages.h"
#include "Net/Server.h"

void show_cl_info();

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //show_cl_info();
    try
    {
        if (rank == 0)
        {
            system("mkdir -p jobs");
            int stations;
            uint64_t minimal_node_size = 500;
            int connection_queue_size = 1000;
            if (argc >= 2)
                minimal_node_size = atoll(argv[1]);
            if (argc >= 3)
                connection_queue_size = atoi(argv[2]);
            MPI_Comm_size(MPI_COMM_WORLD, &stations);
            rank = MPI::Master(static_cast<uint32_t>(stations - 2), minimal_node_size).Run(connection_queue_size);

        }
        else if (rank == 3)
        {
            MPI::JobRequest request;
            request.height = 2000;
            request.width = 2000;
            request.token = 616;
            request.zoom = 2.0;
            request.offset_x = -1.0;
            uint64_t tok = 0;
            Net::Server serv("fractal_server.soc");
            serv.Listen(1);
            Net::Client soc("fractal_cluster.soc");
            sleep(5);
            soc.Connect();
            soc.Write(MPI::MessageID::Request);
            soc.Write(request);
            soc.Close();
            sleep(5);
            do
            {
                soc = serv.Accept();
            } while (!soc.IsConnected());
            soc.Read(tok);
            soc.Close();
            serv.Close();
            Net::Client soc1("fractal_cluster.soc");
            soc1.Connect();
            soc1.Write(MPI::MessageID::Shutdown);
            soc1.Close();
            rank = 0;
        }
        else
            rank = MPI::Slave(rank).Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
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