#include "http_server.h"

int main()
{
    using namespace network;
    HttpServer server = HttpServer("127.0.0.1", 5001);
    server.run();

    return 0;
}
