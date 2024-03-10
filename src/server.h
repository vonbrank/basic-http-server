#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>

namespace network
{
    class IServer
    {
    public:
        virtual void run() = 0;
    };

    class IConnectionHandler
    {
    public:
        virtual void handleConnection(SOCKET &handling_socket) = 0;
    };
} // namespace network

#endif