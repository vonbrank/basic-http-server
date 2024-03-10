#include "http_server.h"
#include "utils.h"
#include <algorithm>
#include <cstddef>
#include <sstream>

namespace network
{
    HttpServer::HttpServer(std::string ip_address, int port) :
        tcpServer(TcpServer(ip_address, port))
    {
        tcpServer.AddConnectionHandler(this);
    }

    HttpServer::~HttpServer()
    {
        tcpServer.RemoveConnectionHandler(this);
    }

    void HttpServer::run()
    {
        tcpServer.run();
    }
    void HttpServer::handleConnection(SOCKET &handling_socket)
    {
        utils::log("[server] Handling connection...");

        readHttpRequest(handling_socket);

        std::string message = "Hello world";

        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\r\n"
           << "Content-Length: " << message.size() << "\r\n"
           << "\r\n"
           << message;

        std::string response_string = ss.str();
        send(handling_socket, response_string.c_str(), response_string.size(), 0);
    }

    void HttpServer::readHttpRequest(SOCKET &handling_socket)
    {
        std::stringstream ss_header;
        std::string ch;
        while (true)
        {
            ch = readBytesFromSocket(handling_socket, 1);
            ss_header << ch;
            if (ss_header.str().find("\r\n\r\n") != std::string::npos)
            {
                break;
            }
        }
        std::string header = ss_header.str();
        ss_header.clear();
        std::string contentLengthKey = "Content-Length: ";
        size_t pos = header.find(contentLengthKey);
        std::string requestBody = "";
        if (pos != std::string::npos)
        {
            pos += contentLengthKey.size();
            size_t end_pos = header.find("\r\n", pos);
            int content_length = std::stoi(header.substr(pos, end_pos - pos));

            requestBody = readBytesFromSocket(handling_socket, content_length);
        }
        utils::log(header);
        utils::log(requestBody);
    }

    std::string HttpServer::readBytesFromSocket(SOCKET &handling_socket, size_t numBytes)
    {
        size_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        size_t totalBytesRead = 0;
        std::ostringstream ss_data;

        while (totalBytesRead < numBytes)
        {
            int bytesRead = recv(handling_socket, buffer, std::min(BUFFER_SIZE - 1, numBytes - totalBytesRead), 0);
            if (bytesRead < 0)
            {
                utils::log("Failed to receive bytes from client socket connection.");
                break;
            }
            else if (bytesRead == 0)
            {
                utils::log("Connection has been closed before finishing reading.");
                break;
            }
            totalBytesRead += bytesRead;
            buffer[bytesRead] = '\0';
            ss_data << buffer;
        }

        return ss_data.str();
    }
} // namespace network