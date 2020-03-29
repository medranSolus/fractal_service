#pragma once
#include "Surface.h"
#include "Window.h"

namespace MPI
{
    enum Channel : int { Jobs, ImageHeaders, ImageData };
    enum class MessageID : uint8_t { Shutdown = 66, Request = 42 };

#pragma pack(push, 1)
    struct JobRequest
    {
        double escape_r = 2.0;
        double zoom = 1.0;
        double offset_x = 0.0;
        double offset_y = 0.0;
        uint32_t width;
        uint32_t height;
        int32_t power = 2;
        uint16_t color_offset = 5;
        uint16_t iterations = 550;
        uint64_t token;
        RGB<float> channels = { 9.0f, 15.0f, 8.5f };
        uint32_t job_id;
        uint32_t min_y;
        uint32_t max_y;
        MessageID id = MessageID::Request;
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