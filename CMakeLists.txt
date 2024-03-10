cmake_minimum_required(VERSION 3.15)
project(basic_http_server CXX)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

aux_source_directory(src DIR_SRC)

add_executable(basic_http_server ${DIR_SRC})



install(TARGETS basic_http_server DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )

target_link_libraries(
                basic_http_server
                PUBLIC wsock32 ws2_32
                PRIVATE Boost::boost)