#pragma once
#include "Messages.h"
#include <mpi.h>

namespace MPI
{
    class Agent
    {
    protected:
        MPI_Datatype work_space_type;
        MPI_Datatype job_request_type;
        MPI_Datatype image_header_type;
        MPI_Datatype color_type;
    
    public:
        Agent() noexcept;
    };
}