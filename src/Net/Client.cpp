#include "Client.h"
#include <thread>

namespace Net
{
    bool Client::Connect()
    {
        if(!connected)
        {
            if (connect(file_desc, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_un)) < 0)
            {
                Logger::LogWarning("Server not responding, retrying to connect...");
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                for (uint8_t i = 1; i <= 5; ++i)
                {
                    if (connect(file_desc, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_un)) >= 0)
                    {
                        Logger::LogInfo("Connected to server after " + std::to_string(i) + " attempts.");
                        connected = true;
                        return true;
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                perror("connect");
                Logger::LogError("Cannot connect to server!");
                return false;
            }
            connected = true;
        }
        return true;
    }

    template<>
    bool Client::Read<MPI::JobRequest>(MPI::JobRequest& value)
    {
        if (connected)
        {
            constexpr size_t request_size = sizeof(value) - 3 * sizeof(uint32_t) - sizeof(value.id);
            if (read(file_desc, &value, request_size) < 0)
            {
                Logger::LogWarning("Socket not responding, retrying to read JobRequest...");
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                for (uint8_t i = 1; i <= 5; ++i)
                {
                    if (read(file_desc, &value, request_size) >= 0)
                    {
                        Logger::LogInfo("Read JobRequest after " + std::to_string(i) + " attempts.");
                        return true;
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                perror("read");
                Logger::LogError("Error reading JobRequest from socket!");
                return false;
            }
            return true;
        }
        Logger::LogError("Cannot read JobRequest from not connected socket!");
        return false;
    }
}