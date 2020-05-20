#pragma once
#include "RGB.h"
#include <stdint.h>

namespace Net
{
    enum class MessageID : uint8_t { Shutdown = 66, RequestClassic = 42, RequestIterative = 43 };

#pragma pack(push, 1)
    struct JobData
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
        RGB<float> channels = { 9.0f, 15.0f, 8.5f };
        uint64_t token;
    };
#pragma pack(pop)
}