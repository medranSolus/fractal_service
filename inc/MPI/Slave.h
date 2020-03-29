#pragma once
#include "Agent.h"
#include "FractalService.h"

namespace MPI
{
    class Slave : public Agent
    {
        int rank;
        FractalService fractals;

        JobRequest ReceiveJob() const;
        void SendImage(const Surface& surface, uint32_t token, uint32_t job_id) const;

    public:
        inline Slave(int rank, const cl::Device& device)
            : rank(rank), fractals(device) {}

        int Run();
    };
}