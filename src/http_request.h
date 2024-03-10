#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <map>
#include <string>

namespace network
{

    class HttpRequest
    {
    public:
        enum class Method
        {
            GET,
            HEAD,
            POST,
            PUT,
            Delete,
            CONNECT,
            OPTIONS,
            TRACE,
            PATCH,
            MAX,
        };

    public:
        HttpRequest(Method method,
                    std::string path,
                    std::string protocolVersion,
                    std::map<std::string, std::string> headers,
                    std::string body);

        static HttpRequest parse(std::string rawHeader, std::string rawBody = "");
        static bool parseHeader(std::string rawHeader, Method &method,
                                std::string &path,
                                std::string &protoclVersion,
                                std::map<std::string, std::string> &headers);

    private:
        Method method;
        std::string path;
        std::string protocolVersion;
        std::map<std::string, std::string> headers;
        std::string body;

    private:
        static Method toMethod(std::string s);
    };
} // namespace network

#endif