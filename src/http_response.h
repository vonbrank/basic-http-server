#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <map>

namespace network
{
    class HttpResponse
    {
    public:
        enum class StatusCode
        {
            OK = 200,
            Created = 201,
            NoContent = 204,
            ResetContent = 205,
            PartialContent = 206,
            Found = 302,
            NotModified = 304,
            BadRequest = 400,
            Unauthorized = 401,
            Forbidden = 403,
            NotFound = 404,
            InternalServerError = 500,
            BadGateway = 502,
            ServiceUnvailable = 503,
            Max = 999,
        };

    public:
        HttpResponse(std::string protocolVersion = "HTTP/1.1",
                     StatusCode statusCode = StatusCode::Max,
                     std::string statusMessage = "",
                     std::map<std::string, std::string> headers = std::map<std::string, std::string>(),
                     std::string body = "");
        std::string encode();
        static HttpResponse make_text(StatusCode statusCode, std::string message);
        static HttpResponse make_allow_methods(StatusCode statusCode);
        StatusCode getStatusCode();
        std::string getHeaderValue(std::string key);

    private:
        std::string protocolVersion;
        StatusCode statusCode;
        std::string statusMessage;
        std::map<std::string, std::string> headers;
        std::string body;

    private:
        static std::string getStatusMessage(StatusCode statusCode);
    };
} // namespace network
#endif