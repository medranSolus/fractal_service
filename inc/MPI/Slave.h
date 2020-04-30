#pragma once
#include "Agent.h"
#include "Logger.h"
#include "FractalService.h"

namespace MPI
{
    class Slave : public Agent
    {
        int rank;
        int master_rank;
        FractalService fractals;

        JobRequest ReceiveJob() const;
        void SendImage(const Surface& surface, uint32_t token, uint32_t job_id) const;

    public:
        inline Slave(int master_rank, int rank, const cl::Device& device)
            : rank(rank), master_rank(master_rank), fractals(device)
        { 
            Logger::LogInfo("Worker " + std::to_string(rank) + " using device: " + device.getInfo<CL_DEVICE_NAME>()); 
        }

        int Run();
    };
}