/****************************************************************************
**
**  ff_yamltool Export Header
**
**  本文件定义了 FF_YAMLTOOL 模块的导入导出宏。
**  在构建动态库时，FF_YAMLTOOL_LIBRARY 会被定义，
**  此时 FF_YAMLTOOL_EXPORT 用于导出符号。
**  在使用该库时，没有定义 FF_YAMLTOOL_LIBRARY，
**  此时 FF_YAMLTOOL_EXPORT 用于导入符号。
**
**  适用于跨平台 (Windows/Linux/macOS) 的 Qt 动态库构建。
**  @Author：zhangzhiming@mozihealthcare.cn
****************************************************************************/
#ifndef FF_YAMLTOOL_EXPORT_H
#define FF_YAMLTOOL_EXPORT_H

#include <QtCore/qglobal.h>

#if defined(FF_YAMLTOOL_LIBRARY)
#  define FF_YAMLTOOL_EXPORT Q_DECL_EXPORT
#else
#  define FF_YAMLTOOL_EXPORT Q_DECL_IMPORT
#endif

#endif // ff_yamltool_EXPORT_H
