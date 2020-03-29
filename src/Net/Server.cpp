#include "Server.h"
#include <exception>

namespace Net
{
    void Server::Listen(int max_connections)
    {
        unlink(address.sun_path);
        if (bind(file_desc, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
        {
            perror("bind");
            Logger::LogError("Cannot bind socket to listen!");
            throw std::string("Cannot bind socket to listen!");
        }
        else if (listen(file_desc, max_connections) < 0)
        {
            perror("listen");
            Logger::LogError("Cannot listen on socket!");
            throw std::string("Cannot listen on socket!");
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
            Logger::LogError("Error accepting incoming call!");
            throw std::string("Error accepting incoming call!");
        }
        return Client(socket , true);
    }
}