#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <vector>
#include <winsock2.h>
#include "server.h"

namespace network
{
    class TcpServer : public IServer, public IConnectionHandler
    {
    public:
        TcpServer(std::string ip_address, int port);
        ~TcpServer();
        void run() override;

        void AddConnectionHandler(IConnectionHandler *connectionHandler);
        void RemoveConnectionHandler(IConnectionHandler *connectionHandler);

    private:
        std::string ip_address;
        int port;

        bool isServerSInitialized;
        bool isServerStarted;

        SOCKET listening_sokcet;
        WSADATA wsaData;
        sockaddr_in socketAddress;
        int socketAddressLen;

        std::vector<IConnectionHandler *> connectionHandlers;

    private:
        void start();
        void close();
        int init();

        void handleConnection(SOCKET &handling_socket) override;
    };
} // namespace network

#endif