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
        HttpRequest(Method method = Method::MAX,
                    std::string path = "",
                    std::string protocolVersion = "",
                    std::map<std::string, std::string> headers = std::map<std::string, std::string>(),
                    std::string body = "");

        static HttpRequest parse(std::string rawHeader, std::string rawBody = "");
        static bool parseHeader(std::string rawHeader, Method &method,
                                std::string &path,
                                std::string &protoclVersion,
                                std::map<std::string, std::string> &headers);
        std::string getHeaderValue(std::string key);
        Method getMethod();
        std::string getPath();
        std::string getProtocolVersion();
        static std::string toString(Method method);

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