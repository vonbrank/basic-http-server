#include "http_response.h"
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <sstream>
#include "utils.h"

namespace network
{
    HttpResponse::HttpResponse(std::string protocolVersion,
                               StatusCode statusCode,
                               std::string statusMessage,
                               std::map<std::string, std::string> headers,
                               std::string body) :
        protocolVersion(protocolVersion),
        statusCode(statusCode),
        statusMessage(statusMessage),
        headers(headers),
        body(body)
    {
        this->headers["Content-Length"] = (boost::format("%1%") % body.size()).str();
        this->headers["Access-Control-Allow-Origin"] = "*";
        this->headers["Access-Control-Allow-Headers"] = "Origin, X-Requested-With, Content-Type, Accept, Authorization";
    }

    std::string HttpResponse::encode()
    {
        std::ostringstream ss_data;

        ss_data << (boost::format("%s %d %s\r\n") % protocolVersion.c_str() % (int)statusCode % statusMessage.c_str()).str();
        for (auto header : headers)
        {
            ss_data << (boost::format("%s: %s\r\n") % header.first.c_str() % header.second.c_str()).str();
        }

        ss_data << "\r\n";
        ss_data << body;

        // utils::log(ss_data.str());

        return ss_data.str();
    }

    HttpResponse HttpResponse::make_text(StatusCode statusCode, std::string message)
    {
        if (message == "")
        {
            return HttpResponse("HTTP/1.1", statusCode, getStatusMessage(statusCode));
        }

        return HttpResponse("HTTP/1.1", statusCode, getStatusMessage(statusCode), boost::assign::map_list_of("Content-Type", "text/plain; charset=us-ascii"), message);
    }
    HttpResponse HttpResponse::make_allow_methods(StatusCode statusCode)
    {
        return HttpResponse("HTTP/1.1", statusCode, getStatusMessage(statusCode), boost::assign::map_list_of("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE"));
    }

    std::string HttpResponse::getStatusMessage(StatusCode statusCode)
    {
        if (statusCode == StatusCode::OK)
        {
            return "OK";
        }
        else if (statusCode == StatusCode::NotFound)
        {
            return "Not Found";
        }
        else if (statusCode == StatusCode::InternalServerError)
        {
            return "Internal Server Error";
        }

        return "";
    }

    HttpResponse::StatusCode HttpResponse::getStatusCode()
    {
        return statusCode;
    }

    std::string HttpResponse::getHeaderValue(std::string key)
    {
        auto it = headers.find(key);

        if (it != headers.end())
        {
            return it->second;
        }

        return "";
    }
} // namespace network