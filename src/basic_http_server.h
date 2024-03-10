#pragma once

#include <vector>
#include <string>

#ifdef _WIN32
    #define BASIC_HTTP_SERVER_EXPORT __declspec(dllexport)
#else
    #define BASIC_HTTP_SERVER_EXPORT
#endif

BASIC_HTTP_SERVER_EXPORT void basic_http_server();
BASIC_HTTP_SERVER_EXPORT void basic_http_server_print_vector(const std::vector<std::string> &strings);
