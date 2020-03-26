#pragma once
#include "Socket.h"
#include "Logger.h"
#include "MPI/Messages.h"

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
                perror("write");
                Logger::LogError("Error writing data to socket!");
                return false;
            }
            return true;
        }
        Logger::LogError("Cannot write to not connected socket!");
        return false;
    }

    template<typename T>
    bool Client::Read(T& value)
    {
        if (connected)
        {
            if (read(file_desc, &value, sizeof(value)) < 0)
            {
                perror("read");
                Logger::LogError("Error reading data from socket!");
                return false;
            }
            return true;
        }
        Logger::LogError("Cannot read from not connected socket!");
        return false;
    }
}