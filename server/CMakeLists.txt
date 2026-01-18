cmake_minimum_required(VERSION 3.15)
project(teaInvS_server)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/utils.cmake)
config_project(ON)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common/logger)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common/yamltool)

add_executable(${PROJECT_NAME} ""
        main.cpp
        include/repo/rootrepo.h
        src/repo/rootrepo.cpp
        include/services/authservice.h
        src/services/authservice.cpp
        include/services/teacherservice.h
        src/services/teacherservice.cpp
        include/repo/reporesult.h)

file(GLOB_RECURSE src CONFIGURE_DEPENDS "*.cpp" "*.h")
target_sources(${PROJECT_NAME} PRIVATE ${src})

find_package(Qt6 REQUIRED COMPONENTS Core NetWork Sql HttpServer WebSockets)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        Qt6::HttpServer
        Qt6::WebSockets
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ff_logger
        ff_yamltool)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

copy_imported_dependencies(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})