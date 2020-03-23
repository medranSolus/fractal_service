#pragma once
#include <mpi.h>

namespace MPI
{
    class Agent
    {
    protected:
        MPI_Datatype image_header;
    
    public:
        Agent() noexcept;
    };
}