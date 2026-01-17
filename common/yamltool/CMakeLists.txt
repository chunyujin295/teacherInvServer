# 日志工具
cmake_minimum_required(VERSION 3.15)
project(ff_yamltool)
include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)

find_package(yaml-cpp REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)

add_standard_module(${PROJECT_NAME} include/yamltool ON ON)
target_link_libraries(${PROJECT_NAME} PUBLIC
        yaml-cpp::yaml-cpp
        Qt6::Core
)
copy_imported_dependencies(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})