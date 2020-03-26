#pragma once
#include "Client.h"
#include <unistd.h>

namespace Net
{
    class Server : public Socket
    {
        using Socket::Socket;

    public:
        virtual inline ~Server() { unlink(address.sun_path); }

        void Listen(int max_connections);
        Client Accept();
    };
}