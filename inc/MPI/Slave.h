#pragma once
#include "Agent.h"

namespace MPI
{
    class Slave : public Agent
    {
        int rank;

        inline JobRequest ReceiveJob() const;
        
        void SendImage(const Surface& surface, uint32_t token, uint32_t job_id) const;

    public:
        inline Slave(int rank) noexcept : rank(rank) {}

        int Run();
    };
}