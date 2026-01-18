/*************************************************
  * 描述：
  *
  * File：stringutil.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_STRINGUTIL_H
#define TEACHERINVSERVER_STRINGUTIL_H

#include <QString>
#include <QStringList>
#include <optional>

namespace StringUtils
{

	// 例："1-3-4" -> {"1","3","4"}
	QStringList splitByChar(const QString& s, QChar sep);

	// 安全 toInt（失败返回 nullopt）
	std::optional<int> toIntSafe(const QString& s);

}

#endif //TEACHERINVSERVER_STRINGUTIL_H