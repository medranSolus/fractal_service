#pragma once
#include "Surface.h"
#include "Window.h"
#include <stdint.h>

namespace MPI
{
    enum Channel : int { Jobs, ImageHeaders, ImageData };
    enum class MessageID : uint8_t { Shutdown, Request };

    struct JobRequest
    {
        MessageID id = MessageID::Request; // Can be also MessageID::Shutdown
        uint16_t iterations;
        uint16_t color_offset;
        uint32_t token;
        uint32_t job_id;
        uint32_t width;
        uint32_t height;
        Window<uint32_t> work_space;
        double escape_r = 2.0;
        double zoom = 1.0;
        double offset = 0.0;
    };

    struct ImageHeader
    {
        uint32_t token;
        uint32_t job_id;
        uint32_t width;
        uint32_t height;
    };

    struct JobResponse
    {
        uint32_t token;
        uint32_t job_id;
        Surface image;
    };
}