#include "Agent.h"

namespace MPI
{
    Agent::Agent() noexcept
    {
        MPI_Type_contiguous(2, MPI_UINT32_T, &image_header);
        MPI_Type_commit(&image_header);
    }
}