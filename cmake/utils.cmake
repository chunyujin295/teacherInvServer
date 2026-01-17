#------------------------------- 函数 --------------------------------
# \brief 函数名称 : copy_target_dependencies【拷贝目标依赖】
# @target      : 目标
# @visited     : 避免循环依赖
function(copy_imported_dependencies target dest_dir)
#    if (COPY_3RD_LIB)
        get_imported_shared_libs(${target} deps)

        foreach (lib IN LISTS deps)
            file(COPY ${lib} DESTINATION ${dest_dir})
            message(STATUS "已拷贝依赖: ${lib} -> ${dest_dir}")
        endforeach ()
#    endif (COPY_3RD_LIB)
endfunction()



# \brief 函数名称 : copy_qt_libs【安装Qt库】
# @QT_LIBS      : Qt库列表
# @dest_dir     : 安装目录
function(copy_qt_libs dest_dir)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs QT_LIBS)
    cmake_parse_arguments(INSTALL_QT_LIBS
            "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(DEBUG_SUFFIX)
        if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
            set(DEBUG_SUFFIX "d")
        endif ()

        # 推导 Qt 安装目录
        set(QT_INSTALL_PATH "${Qt6_DIR}/../../..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
                set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            endif ()
        endif ()

        # 平台插件 (Windows 必需)
        if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
            execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
                    "${dest_dir}/plugins/platforms/")
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                    "${dest_dir}/plugins/platforms/")
        endif ()

        # Qt 库和插件目录映射（相对完整）
        set(QT_PLUGIN_MAP
                Gui "platforms\;imageformats\;iconengines\;platforminputcontexts\;styles"
                Widgets "platforms\;imageformats\;iconengines\;platforminputcontexts\;styles"
                Network "bearer"
                Sql "sqldrivers"
                Svg "imageformats"
                Multimedia "mediaservice\;audio"
                MultimediaWidgets "mediaservice\;audio"
                Qml "qmltooling"
                Quick "scenegraph\;qmltooling"
                Positioning "position"
                Sensors "sensors"
                Bluetooth "bluetooth"
                PrintSupport "printsupport"
                WebEngineCore "resources\;translations"
                WebView "resources\;translations"
                TextToSpeech "texttospeech"
                VirtualKeyboard "virtualkeyboard"
        )

        # 拷贝库和插件
        foreach (QT_LIB ${INSTALL_QT_LIBS_QT_LIBS})
            # DLL
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "${dest_dir}")

            # 查找插件映射
            list(FIND QT_PLUGIN_MAP ${QT_LIB} plugin_index)
            if (NOT plugin_index EQUAL -1)
                math(EXPR plugin_value_index "${plugin_index} + 1")
                list(GET QT_PLUGIN_MAP ${plugin_value_index} plugin_dirs_str)
                string(REPLACE ";" "|" plugin_dirs_pattern "${plugin_dirs_str}") # 防止 split 被误解
                string(REPLACE ";" ";" plugin_dirs "${plugin_dirs_str}")

                foreach (plugin_dir ${plugin_dirs})
                    set(src_plugin_dir "${QT_INSTALL_PATH}/plugins/${plugin_dir}")
                    set(dst_plugin_dir "${dest_dir}/plugins/${plugin_dir}")

                    if (EXISTS "${src_plugin_dir}")
                        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${dst_plugin_dir}")
                        file(GLOB plugin_files "${src_plugin_dir}/*${DEBUG_SUFFIX}.dll")
                        foreach (plugin_file ${plugin_files})
                            execute_process(COMMAND ${CMAKE_COMMAND} -E copy
                                    "${plugin_file}" "${dst_plugin_dir}")
                        endforeach ()
                    endif ()
                endforeach ()
            endif ()
        endforeach ()
    endif ()
endfunction()

#------------------------------- 宏 ----------------------------------

# \brief 宏名称    : add_standard_module【CXI添加标准模块宏】
# @target         : 库目标
# @export_location: 导出位置
# @is_shared      : 是否为动态库
# @is_qt_module   : 是否依赖Qt
macro(add_standard_module target export_location is_shared is_qt_module)
    if (${target} STREQUAL "")
        message(FATAL_ERROR "${target}目标不存在")
    endif ()
    if (NOT ${is_shared})
        message(FATAL_ERROR "动静态库配置参数不存在")
    endif ()
    if (NOT ${is_qt_module})
        message(FATAL_ERROR "Qt配置参数不存在")
    endif ()

    config_project(${is_qt_module})
    if (${is_qt_module})
        add_library(${target} SHARED)
    else ()
        add_library(${target})
    endif ()
    if (NOT ${export_location} STREQUAL "")
        export_symbol(${target} ${export_location})
    endif ()
    file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
    file(GLOB_RECURSE srcs_p CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/private/*.h")
    file(GLOB_RECURSE uis CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/uis/*.ui")
    file(GLOB_RECURSE qrcs CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/res/*.qrc")
    target_include_directories(${target} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private")
    target_sources(${target} PRIVATE ${srcs} ${uis} ${srcs_p} ${qrcs})
endmacro()