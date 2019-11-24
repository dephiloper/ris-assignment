cmake_minimum_required(VERSION 3.15)
project(ogl_getting_started)

set(CMAKE_CXX_STANDARD 17)

include_directories("/usr/include/" "/usr/include/glm" "lib")
add_executable(ogl_getting_started main.cpp Shader.h Shader.cpp Camera.h Camera.cpp lib/glad/glad.c lib/stb/stb_image.h lib/stb/stb_image.h)

target_link_libraries(ogl_getting_started "glfw" ${CMAKE_DL_LIBS})