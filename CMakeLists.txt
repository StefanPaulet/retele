cmake_minimum_required(VERSION 3.23)
project(concurrent_sv)

set(CMAKE_CXX_STANDARD 20)

set ( CMAKE_CXX_FLAGS -pthread )

set (
        PROJ_INCLUDE_DIRECTORIES
        include
)

include_directories  ( ${PROJ_INCLUDE_DIRECTORIES} )

find_package ( Qt5Widgets REQUIRED )

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)
add_executable(main main.cpp)

target_link_libraries ( main PRIVATE Qt5::Widgets )
