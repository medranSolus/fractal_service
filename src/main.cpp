#include "MPI/Master.h"
#include "MPI/Slave.h"
#include <utility>

cl::Device get_cl_device(int rank);

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    try
    {
        int master_rank = 0;
        if (argc >= 2)
            master_rank = atoi(argv[1]);
        if (rank == master_rank)
        {
            system("mkdir -p jobs");
            int stations;
            uint64_t minimal_node_size = 500;
            int connection_queue_size = 1000;
            if (argc >= 3)
                minimal_node_size = abs(atoll(argv[2]));
            if (argc == 4)
                connection_queue_size = atoi(argv[3]);
            MPI_Comm_size(MPI_COMM_WORLD, &stations);
            rank = MPI::Master(master_rank, static_cast<uint32_t>(stations - 1), minimal_node_size).Run(connection_queue_size);

        }
        else
            rank = MPI::Slave(master_rank, rank, get_cl_device(rank)).Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    MPI_Finalize();
    return rank;
}

cl::Device get_cl_device(int rank)
{
    std::vector<cl::Device> all_devices;
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for (auto& platform : platforms)
    {
        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        for (auto& device : devices)
            all_devices.emplace_back(device);
    }
    return all_devices.at(rank % all_devices.size());
}