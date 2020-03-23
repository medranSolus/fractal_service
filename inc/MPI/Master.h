#pragma once
#include "Agent.h"

namespace MPI
{
    class Master : public Agent
    {
        int stations;

    public:
        inline Master(int stations) noexcept : stations(stations) {}

        int Run();
    };
}