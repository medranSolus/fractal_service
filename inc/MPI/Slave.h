#pragma once
#include "Agent.h"

namespace MPI
{
    class Slave : public Agent
    {
        int rank;

    public:
        inline Slave(int rank) noexcept : rank(rank) {}

        int Run();
    };
}