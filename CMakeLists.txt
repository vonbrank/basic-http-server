cmake_minimum_required(VERSION 3.15)
project(basic_http_server CXX)



add_executable(basic_http_server src/basic_http_server.cpp src/main.cpp src/tcp_server.cpp src/utils.cpp src/http_server.cpp)



install(TARGETS basic_http_server DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )

target_link_libraries(basic_http_server wsock32 ws2_32)