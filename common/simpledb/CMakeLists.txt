cmake_minimum_required(VERSION 3.15)
project(ff_simpledb)
include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)

find_package(Qt6 COMPONENTS Core Sql REQUIRED)

add_standard_module(${PROJECT_NAME} include/simpledb ON ON)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Sql)

copy_imported_dependencies(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})