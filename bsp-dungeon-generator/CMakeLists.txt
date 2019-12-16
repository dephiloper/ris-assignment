cmake_minimum_required(VERSION 3.15)
project(bsp-dungeon-generator)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)


target_link_libraries(main ${CMAKE_DL_LIBS})
