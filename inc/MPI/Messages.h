#pragma once
#include "Surface.h"
#include "Window.h"
#include <stdint.h>

namespace MPI
{
    enum Channel : int { Jobs, ImageHeaders, ImageData };
    enum class MessageID : uint8_t { Shutdown = 66, Request = 42 };

#pragma pack(push, 1)
    struct JobRequest
    {
        long double escape_r = 2.0;
        long double zoom = 1.0;
        long double offset_x = 0.0;
        long double offset_y = 0.0;
        uint32_t width;
        uint32_t height;
        int32_t power = 2;
        uint16_t color_offset = 7;
        uint16_t iterations = 550;
        uint64_t token;
        RGB<float> channels = { 9.0f, 15.0f, 8.5f };
        uint32_t job_id;
        Window<uint32_t> work_space;
        MessageID id = MessageID::Request; // Can be also MessageID::Shutdown
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