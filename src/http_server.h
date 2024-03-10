#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "server.h"
#include <cstddef>
#include <string>
#include "tcp_server.h"
#include "http_request.h"

namespace network
{
    class HttpServer : public IServer, public IConnectionHandler
    {
    public:
        HttpServer(std::string ip_address, int port);
        ~HttpServer();
        void run() override;

    private:
        TcpServer tcpServer;

    private:
        void handleConnection(SOCKET &handling_socket) override;
        bool readHttpRequest(SOCKET &handling_socket, HttpRequest &outHttpRequest);
        std::string readBytesFromSocket(SOCKET &handling_socket, size_t numBytes);
    };
} // namespace network

#endif