#include "http_server.h"
#include "utils.h"
#include <algorithm>
#include <cstddef>
#include <sstream>
#include "http_request.h"
#include "http_response.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

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
        utils::log("[http-server] Handling connection...");

        while (true)
        {
            HttpRequest httpRequest = HttpRequest();
            if (readHttpRequest(handling_socket, httpRequest))
            {
                auto httpResponse = HttpResponse();

                if (httpRequest.getMethod() == HttpRequest::Method::GET)
                {
                    std::string message = "Hello world";
                    httpResponse = HttpResponse::make_text(HttpResponse::StatusCode::OK, message);
                }
                else if (httpRequest.getMethod() == HttpRequest::Method::OPTIONS)
                {
                    httpResponse = HttpResponse::make_allow_methods(HttpResponse::StatusCode::OK);
                }
                else
                {
                    httpResponse = HttpResponse::make_text(HttpResponse::StatusCode::InternalServerError, "");
                }

                std::string response_string = httpResponse.encode();
                // utils::log(response_string);
                send(handling_socket, response_string.c_str(), response_string.size(), 0);
                auto contentLength = httpResponse.getHeaderValue("Content-Length");
                utils::log((boost::format("%s %s %d - %s")
                            % HttpRequest::toString(httpRequest.getMethod()).data()
                            % httpRequest.getPath().data() % (int)httpResponse.getStatusCode()
                            % (contentLength.size() ? contentLength : "-").data())
                               .str());

                if (boost::algorithm::to_lower_copy(httpRequest.getHeaderValue("Connection")) == "keep-alive")
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        utils::log("[http-server] Connection closed...");
    }

    bool HttpServer::readHttpRequest(SOCKET &handling_socket, HttpRequest &outHttpRequest)
    {
        std::stringstream ss_header;
        std::string ch;
        while (true)
        {
            ch = readBytesFromSocket(handling_socket, 1);
            if (ch.size() == 0)
            {
                return false;
            }
            ss_header << ch;
            if (ss_header.str().find("\r\n\r\n") != std::string::npos)
            {
                break;
            }
        }

        std::string header = ss_header.str();

        HttpRequest::Method method = HttpRequest::Method::MAX;
        std::string path = "";
        std::string protoclVersion = "";
        std::map<std::string, std::string> headers;
        network::HttpRequest::parseHeader(header, method, path, protoclVersion, headers);

        std::string requestBody = "";
        auto contentLengthIt = headers.find("Content-Length");
        if (contentLengthIt != headers.end())
        {
            auto contentLengthString = contentLengthIt->second;
            int content_length = std::stoi(contentLengthString);

            requestBody = readBytesFromSocket(handling_socket, content_length);
        }

        outHttpRequest = HttpRequest::parse(header, requestBody);

        return true;
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
                if (totalBytesRead)
                {
                    utils::log("Connection has been closed before finishing reading.");
                }
                break;
            }
            totalBytesRead += bytesRead;
            buffer[bytesRead] = '\0';
            ss_data << buffer;
        }

        return ss_data.str();
    }
} // namespace network