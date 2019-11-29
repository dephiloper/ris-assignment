cmake_minimum_required(VERSION 3.15)
project(network_getting_started)

set(CMAKE_CXX_STANDARD 17)


include_directories("/usr/include/")

add_executable(client src/client.cpp src/netmsg.h)
add_executable(server src/server.cpp src/netmsg.h)

target_link_libraries(client "glfw" "zmq" "pthread" ${CMAKE_DL_LIBS})
target_link_libraries(server "glfw" "zmq" "pthread" ${CMAKE_DL_LIBS})
