#pragma once
#include "Surface.h"
#include "Net/Data.h"

namespace MPI
{
    enum Channel : int { Jobs, ImageHeaders, ImageData };

#pragma pack(push, 1)
    struct JobRequest
    {
        Net::JobData data;
        uint32_t job_id;
        uint32_t min_y;
        uint32_t max_y;
        Net::MessageID id = Net::MessageID::RequestClassic;
    };

    struct ImageHeader
    {
        uint64_t token;
        uint32_t job_id;
        uint32_t width;
        uint32_t height;
    };
#pragma pack(pop)

    struct JobResponse
    {
        uint64_t token;
        uint32_t job_id;
        std::shared_ptr<Surface> image;
    };
}