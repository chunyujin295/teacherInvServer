include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)

# 相当于设置上-DQt6_DIR='/lib/cmake/Qt6'，但是通过这种方式，变成缓存变量，对GUI更友好且复用性更高
set(Qt6_DIR "" CACHE PATH "Path to Qt6Config.cmake directory (e.g. /lib/cmake/Qt6)")
#find_package(Qt6 COMPONENTS Core NetWork REQUIRED)

# 追加CMAKE_PREFIX_PATH，将路径纳入find_package的查找环境中
if (WIN32)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rdparty/win64")
elseif (UNIX)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rdparty/linux64")
endif()

copy_qt_libs(${CMAKE_RUNTIME_OUTPUT_DIRECTORY} QT_LIBS
        Core
#        Gui
#        Widgets
#        Test
#        Designer
        Network
        Sql
)

copy_dir_files(${CMAKE_SOURCE_DIR}/3rdparty/db ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )



