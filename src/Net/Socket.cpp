#include "Socket.h"
#include "Logger.h"
#include <stdexcept>

namespace Net
{
    Socket::Socket(const std::string& name)
    {
        file_desc = socket(AF_UNIX, SOCK_STREAM, 0);
        if (file_desc < 0)
        {
            perror("socket");
            const std::string error_msg = "Cannot create socket: " + name;
            Logger::LogError(error_msg);
            throw std::runtime_error(error_msg);
        }
        else
        {
            memset(&address, 0, sizeof(address));
            address.sun_family = AF_UNIX;
            strncpy(address.sun_path, name.c_str(), sizeof(address.sun_path) - 1);
        }
    }
}