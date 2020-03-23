#pragma once
#include <stdint.h>

namespace MPI
{
    enum class MessageID : uint8_t { Shutdown, JobRequest };

    struct JobRequest
    {
        MessageID id = MessageID::JobRequest; // Can be also MessageID::Shutdown
        uint32_t token;
        uint32_t width;
        uint32_t height;
        double escape_r = 2.0;
        double zoom = 1.0;
        double offset = 0.0;
    };

    struct ImageHeader
    {
        uint32_t token;
        uint32_t width;
        uint32_t height;
    };
}