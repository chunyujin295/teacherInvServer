# 日志工具
cmake_minimum_required(VERSION 3.15)
project(ff_logger)
include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)

find_package(spdlog REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)

add_standard_module(${PROJECT_NAME} include/logger ON ON)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
        ff_yamltool
)
copy_imported_dependencies(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})