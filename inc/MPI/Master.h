#pragma once
#include "Agent.h"

namespace MPI
{
    class Master : public Agent
    {
        int stations;

        inline void SendJob(int rank, const JobRequest& request) const { MPI_Send(&request, 1, job_request_type, rank, Channel::Jobs, MPI_COMM_WORLD); }

        JobResponse ReceiveImage() const;

    public:
        inline Master(int stations) noexcept : stations(stations) {}

        int Run();
    };
}