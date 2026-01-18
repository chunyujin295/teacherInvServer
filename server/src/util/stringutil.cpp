#include <util/stringutil.h>

namespace StringUtils
{

	QStringList splitByChar(const QString& s, QChar sep)
	{
		return s.split(sep, Qt::SkipEmptyParts);
	}

	std::optional<int> toIntSafe(const QString& s)
	{
		bool ok = false;
		int v = s.trimmed().toInt(&ok);
		if (!ok) return std::nullopt;
		return v;
	}

}// namespace StringUtils