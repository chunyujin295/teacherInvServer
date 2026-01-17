/****************************************************************************
**
**  ff_logger Export Header
**
**  本文件定义了 FF_LOGGER 模块的导入导出宏。
**  在构建动态库时，FF_LOGGER_LIBRARY 会被定义，
**  此时 FF_LOGGER_EXPORT 用于导出符号。
**  在使用该库时，没有定义 FF_LOGGER_LIBRARY，
**  此时 FF_LOGGER_EXPORT 用于导入符号。
**
**  适用于跨平台 (Windows/Linux/macOS) 的 Qt 动态库构建。
**  @Author：zhangzhiming@mozihealthcare.cn
****************************************************************************/
#ifndef FF_LOGGER_EXPORT_H
#define FF_LOGGER_EXPORT_H

#include <QtCore/qglobal.h>

#if defined(FF_LOGGER_LIBRARY)
#  define FF_LOGGER_EXPORT Q_DECL_EXPORT
#else
#  define FF_LOGGER_EXPORT Q_DECL_IMPORT
#endif

#endif // ff_logger_EXPORT_H
