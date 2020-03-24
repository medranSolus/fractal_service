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
        long double escape_r = 2.0;
        long double zoom = 1.0;
        long double offset_x = 0.0;
        long double offset_y = 0.0;
        Window<uint32_t> work_space;
        uint32_t width;
        uint32_t height;
        uint32_t token;
        uint32_t job_id;
        int power;
        uint16_t color_offset;
        uint16_t iterations;
        MessageID id = MessageID::Request; // Can be also MessageID::Shutdown
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