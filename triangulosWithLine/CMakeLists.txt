cmake_minimum_required(VERSION 3.13)
project(triangulosWithLine)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(triangulosWithLine
        CMakeLists.txt
        main.cpp)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(Cocoa REQUIRED)
find_library(CoreVideo REQUIRED)
find_library(IOKit REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(triangulosWithLine /usr/local/lib/libglfw.3.3.dylib ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(triangulosWithLine PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} /usr/local/include/glm)
