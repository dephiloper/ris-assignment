cmake_minimum_required(VERSION 3.15)
project(cubeshot)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE cubeshot_headers "${CMAKE_CURRENT_LIST_DIR}/src/*.h")
file(GLOB_RECURSE cubeshot_sources "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE lib_headers "${CMAKE_CURRENT_LIST_DIR}/lib/*.h")
file(GLOB_RECURSE lib_sources "${CMAKE_CURRENT_LIST_DIR}/lib/*.c" "${CMAKE_CURRENT_LIST_DIR}/lib/*.cpp")

include_directories("/usr/include/" "/usr/include/glm" "lib")

add_executable(cubeshot ${lib_headers} ${lib_sources} ${cubeshot_headers} ${cubeshot_sources})

target_link_libraries(cubeshot "glfw" ${CMAKE_DL_LIBS})