/*************************************************
  * 描述：
  *
  * File：timeutil.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_TIMEUTIL_H
#define TEACHERINVSERVER_TIMEUTIL_H

#include <QString>
#include <QDateTime>
#include <optional>

namespace TimeUtils {

	// 你的旧格式：YYYY-M-D#H:MM（原代码里是用 '-' 和 '#', ':' 拼出来的）
	// 我们统一支持：YYYY-MM-DD#HH:MM（也兼容 1 位月/日/时）
	std::optional<QDateTime> parseLegacy(const QString& s);

	// 转回 legacy 字符串（用于落库/返回）
	QString toLegacy(const QDateTime& dt);

	bool isFuture(const QDateTime& dt);

	// end 必须 >= start，否则返回 std::nullopt
	std::optional<int> minutesBetween(const QDateTime& start, const QDateTime& end);

}

#endif //TEACHERINVSERVER_TIMEUTIL_H