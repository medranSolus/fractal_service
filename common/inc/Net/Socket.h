#pragma once
#include <string>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

namespace Net
{
    class Socket
    {
    protected:
        int file_desc;
        sockaddr_un address;

        constexpr Socket(int file_desc) noexcept : file_desc(file_desc), address({ AF_UNIX }) {}

    public:
        Socket(const std::string& name);
        virtual ~Socket() = default;
        
        inline void Close() { close(file_desc); }
    };
}