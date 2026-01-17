cmake_minimum_required(VERSION 3.15)
project(teaInvS_server)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common/logger/ff_logger)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common/yamltool/ff_yamltool)

add_executable(${PROJECT_NAME} "")

find_package(Qt6 REQUIRED COMPONENTS Core NetWork)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Network
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ff_logger
        ff_yamltool)

copy_imported_dependencies(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})