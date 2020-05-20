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
                Logger::LogError("Cannot connect to server! Socket: " + std::string(address.sun_path));
                return false;
            }
            connected = true;
        }
        return true;
    }
}