#include "Server.h"
#include <stdexcept>

namespace Net
{
    void Server::Listen(int max_connections)
    {
        unlink(address.sun_path);
        if (bind(file_desc, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
        {
            perror("bind");
            const std::string error_msg = "Cannot bind socket to listen! Socket: " + std::string(address.sun_path);
            Logger::LogError(error_msg);
            throw std::runtime_error(error_msg);
        }
        else if (listen(file_desc, max_connections) < 0)
        {
            perror("listen");
            const std::string error_msg = "Cannot listen on socket! Socket: " + std::string(address.sun_path);
            Logger::LogError(error_msg);
            throw std::runtime_error(error_msg);
        }
    }

    Client Server::Accept()
    {
        int socket = accept4(file_desc, nullptr, nullptr, SOCK_NONBLOCK);
        if (socket == EAGAIN || socket == EWOULDBLOCK)
            return Client(-1 , false);
        if (socket < 0)
        {
            perror("accept");
            const std::string error_msg = "Error accepting incoming call! Socket: " + std::string(address.sun_path);
            Logger::LogError(error_msg);
            throw std::runtime_error(error_msg);
        }
        return Client(socket , true);
    }
}