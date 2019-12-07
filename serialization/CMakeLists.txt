cmake_minimum_required(VERSION 3.15)
project(network_getting_started)

set(CMAKE_CXX_STANDARD 17)

include_directories("/usr/include/" "lib")

add_executable(main src/main.cpp)

target_link_libraries(main "zmq" "pthread" ${CMAKE_DL_LIBS})
