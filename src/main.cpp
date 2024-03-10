#include "basic_http_server.h"
#include <vector>
#include <string>

int main()
{
    basic_http_server();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    basic_http_server_print_vector(vec);
}
