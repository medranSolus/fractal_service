#pragma once
#include "Socket.h"
#include "Logger.h"
#include <chrono>
#include <thread>

namespace Net
{
    class Client : public Socket
    {
        friend class Server;
        using Socket::Socket;

        bool connected = false;
    
        constexpr Client(int file_desc, bool connected) noexcept : Socket(file_desc), connected(connected) {}

    public:
        virtual ~Client() = default;

        constexpr bool IsConnected() const noexcept { return connected; }

        bool Connect();
        
        template<typename T>
        bool Write(const T& value);
        template<typename T>
        bool Read(T& value);
    };

    template<typename T>
    bool Client::Write(const T& value)
    {
        if (connected)
        {
            if (write(file_desc, &value, sizeof(value)) < 0)
            {
                Logger::LogWarning("Socket not responding, retrying to send " + std::to_string(sizeof(value)) + " bytes...");
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                for (uint8_t i = 1; i <= 5; ++i)
                {
                    if (write(file_desc, &value, sizeof(value)) >= 0)
                    {
                        Logger::LogInfo("Send " + std::to_string(sizeof(value)) + " bytes after " + std::to_string(i) + " attempts.");
                        return true;
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                perror("write");
                Logger::LogError("Error sending " + std::to_string(sizeof(value)) + " bytes to socket: " + std::string(address.sun_path));
                return false;
            }
            return true;
        }
        Logger::LogError("Cannot write to not connected socket: " + std::string(address.sun_path));
        return false;
    }

    template<typename T>
    bool Client::Read(T& value)
    {
        if (connected)
        {
            if (read(file_desc, &value, sizeof(value)) < 0)
            {
                Logger::LogWarning("Socket not responding, retrying to read " + std::to_string(sizeof(value)) + " bytes...");
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                for (uint8_t i = 1; i <= 5; ++i)
                {
                    if (read(file_desc, &value, sizeof(value)) >= 0)
                    {
                        Logger::LogInfo("Read " + std::to_string(sizeof(value)) + " bytes after " + std::to_string(i) + " attempts.");
                        return true;
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                perror("read");
                Logger::LogError("Error reading " + std::to_string(sizeof(value)) + " bytes from socket: " + std::string(address.sun_path));
                return false;
            }
            return true;
        }
        Logger::LogError("Cannot read from not connected socket: " + std::string(address.sun_path));
        return false;
    }
}