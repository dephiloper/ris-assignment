cmake_minimum_required(VERSION 3.15)
project(network_getting_started)

set(CMAKE_CXX_STANDARD 17)

include_directories("/usr/include/")

add_executable(client src/client.cpp src/clientnetmanager.cpp src/clientnetmanager.h src/netmanager.cpp src/netmanager.h src/netmessage.h src/netmessage.cpp src/blockingqueue.h)
add_executable(server src/server.cpp src/servernetmanager.cpp src/servernetmanager.h src/netmanager.cpp src/netmanager.h src/netmessage.h src/netmessage.cpp src/blockingqueue.h)

target_link_libraries(client "zmq" "pthread" ${CMAKE_DL_LIBS})
target_link_libraries(server "zmq" "pthread" ${CMAKE_DL_LIBS})
