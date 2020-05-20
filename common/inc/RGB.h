#pragma once

#pragma pack(push, 1)
template<typename T>
struct RGB
{
    T r = static_cast<T>(0);
    T g = static_cast<T>(0);
    T b = static_cast<T>(0);
};
#pragma pack(pop)