cmake_minimum_required(VERSION 3.10.0)
project(xviz_demo)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_SOURCE_DIR}/bin)

file(COPY xviz/resources DESTINATION ${CMAKE_SOURCE_DIR}/bin)
file(COPY xviz/bin DESTINATION ${CMAKE_SOURCE_DIR})

# 对于python 需要把所有库都放在一起
file(GLOB ALL_LIBS xviz/lib/*)
foreach(file ${ALL_LIBS})
    file(COPY ${file} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endforeach()
include_directories( xviz/include)
link_directories(xviz/lib)


set(libxviz XvizAppLib fmtd glad glfw3dll imguid implotd spdlogd yaml-cppd)

add_executable(demo_layer demo/demo_layer/demo_layer.cpp)
target_link_libraries(demo_layer PRIVATE ${libxviz})


add_executable(xviz_demo demo/xviz_demo/main.cpp demo/xviz_demo/demo.cpp)
target_link_libraries(xviz_demo PRIVATE ${libxviz})


