#include <util/timeutil.h>
#include <QRegularExpression>

namespace TimeUtils {

	std::optional<QDateTime> parseLegacy(const QString& s) {
		// 支持：2023-11-20#14:30 或 2023-1-2#3:04
		static QRegularExpression re(
		  R"(^(?<y>\d{4})-(?<m>\d{1,2})-(?<d>\d{1,2})#(?<hh>\d{1,2}):(?<mm>\d{1,2})$)"
		);
		auto m = re.match(s.trimmed());
		if (!m.hasMatch()) return std::nullopt;

		const int y = m.captured("y").toInt();
		const int mo = m.captured("m").toInt();
		const int d = m.captured("d").toInt();
		const int hh = m.captured("hh").toInt();
		const int mm = m.captured("mm").toInt();

		QDate date(y, mo, d);
		QTime time(hh, mm);
		if (!date.isValid() || !time.isValid()) return std::nullopt;

		return QDateTime(date, time);
	}

	QString toLegacy(const QDateTime& dt) {
		// 固定成 2 位：YYYY-MM-DD#HH:MM
		return dt.toString("yyyy-MM-dd#HH:mm");
	}

	bool isFuture(const QDateTime& dt) {
		return dt > QDateTime::currentDateTime();
	}

	std::optional<int> minutesBetween(const QDateTime& start, const QDateTime& end) {
		if (end < start) return std::nullopt;
		return int(start.secsTo(end) / 60);
	}

} // namespace TimeUtils
