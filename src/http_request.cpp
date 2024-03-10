#include "http_request.h"
#include <map>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>
#include "utils.h"

namespace network
{
    HttpRequest::HttpRequest(Method method,
                             std::string path,
                             std::string protocolVersion,
                             std::map<std::string, std::string> headers,
                             std::string body) :
        method(method),
        path(path),
        protocolVersion(protocolVersion),
        headers(headers),
        body(body)
    {
    }

    HttpRequest HttpRequest::parse(std::string rawHeader, std::string rawBody)
    {
        std::vector<std::string> split_header_lines;
        boost::split(split_header_lines, rawHeader, boost::is_any_of("\r\n"), boost::token_compress_on);

        Method method = Method::MAX;
        std::string path = "";
        std::string protoclVersion = "";
        std::map<std::string, std::string> headers;

        parseHeader(rawHeader, method, path, protoclVersion, headers);

        return HttpRequest(
            method,
            path, protoclVersion,
            headers,
            rawBody);
    }

    bool HttpRequest::parseHeader(std::string rawHeader, Method &method,
                                  std::string &path,
                                  std::string &protoclVersion,
                                  std::map<std::string, std::string> &headers)
    {
        bool res = true;
        method = Method::MAX;
        std::vector<std::string> split_header_lines;
        boost::split(split_header_lines, rawHeader, boost::is_any_of("\r\n"), boost::token_compress_on);
        for (int i = 0; i < split_header_lines.size(); i++)
        {
            auto split_header_line = split_header_lines[i];
            if (i == 0)
            {
                std::vector<std::string> split_first_header_line_tokens;
                boost::split(split_first_header_line_tokens, split_header_line, boost::is_any_of(" "), boost::token_compress_on);
                if (split_first_header_line_tokens.size() >= 3)
                {
                    method = toMethod(split_first_header_line_tokens[0]);
                    path = split_first_header_line_tokens[1];
                    protoclVersion = split_first_header_line_tokens[2];
                }
            }
            else
            {
                std::vector<std::string> split_header_line_tokens;
                // utils::log(split_header_line);
                boost::split(split_header_line_tokens, split_header_line, boost::is_any_of(":"), boost::token_compress_on);
                if (split_header_line_tokens.size() >= 2)
                {
                    headers[split_header_line_tokens[0]] = boost::trim_copy(split_header_line_tokens[1]);
                    // utils::log((boost::format("key=%s, value=%s") % split_header_line_tokens[0].c_str() % headers[split_header_line_tokens[0]].c_str()).str());
                }
            }
        }

        if (method == Method::MAX)
        {
            res = false;
        }

        return res;
    }

    HttpRequest::Method HttpRequest::toMethod(std::string s)
    {
        if (s == "GET")
        {
            return HttpRequest::Method::GET;
        }
        else if (s == "HEAD")
        {
            return HttpRequest::Method::HEAD;
        }
        else if (s == "POST")
        {
            return HttpRequest::Method::POST;
        }
        else if (s == "PUT")
        {
            return HttpRequest::Method::PUT;
        }
        else if (s == "DELETE")
        {
            return HttpRequest::Method::Delete;
        }
        else if (s == "CONNECT")
        {
            return HttpRequest::Method::CONNECT;
        }
        else if (s == "OPTIONS")
        {
            return HttpRequest::Method::OPTIONS;
        }
        else if (s == "TRACE")
        {
            return HttpRequest::Method::TRACE;
        }
        else if (s == "PATCH")
        {
            return HttpRequest::Method::PATCH;
        }

        return HttpRequest::Method::MAX;
    }

    std::string HttpRequest::getHeaderValue(std::string key)
    {
        auto it = headers.find(key);

        if (it != headers.end())
        {
            return it->second;
        }

        return "";
    }

} // namespace network