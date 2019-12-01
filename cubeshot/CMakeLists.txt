cmake_minimum_required(VERSION 3.15)
project(cubeshot)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE client_sources "${CMAKE_CURRENT_LIST_DIR}/src/client/*.h" "${CMAKE_CURRENT_LIST_DIR}/src/client/*.cpp")
file(GLOB_RECURSE server_sources "${CMAKE_CURRENT_LIST_DIR}/src/server/*.h" "${CMAKE_CURRENT_LIST_DIR}/src/server/*.cpp")
file(GLOB_RECURSE shared_sources "${CMAKE_CURRENT_LIST_DIR}/src/shared/*.h" "${CMAKE_CURRENT_LIST_DIR}/src/shared/*.cpp")

file(GLOB_RECURSE lib_sources "${CMAKE_CURRENT_LIST_DIR}/lib/*.h" "${CMAKE_CURRENT_LIST_DIR}/lib/*.c" "${CMAKE_CURRENT_LIST_DIR}/lib/*.cpp")

include_directories("/usr/include/" "/usr/include/glm" "lib")

add_executable(client_cubeshot ${lib_sources} ${client_sources} ${shared_sources})
add_executable(server_cubeshot ${lib_sources} ${server_sources} ${shared_sources})

target_link_libraries(client_cubeshot "glfw" "pthread" "zmq" ${CMAKE_DL_LIBS})
target_link_libraries(server_cubeshot "glfw" "pthread" "zmq" ${CMAKE_DL_LIBS})