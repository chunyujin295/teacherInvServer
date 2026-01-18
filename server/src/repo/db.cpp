#include <repo/db.h>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

static const char* kConnName = "main";

bool Db::initSqlite(const QString& sqlitePath)
{
	if (QSqlDatabase::contains(kConnName))
	{
		return true;
	}

	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", kConnName);
	database.setDatabaseName(sqlitePath);

	if (!database.open())
	{
		qCritical() << "Open sqlite failed:" << database.lastError().text();
		return false;
	}

	// SQLite 建议设置：WAL + foreign_keys
	QSqlQuery q(database);

	if (!q.exec("PRAGMA foreign_keys = ON;"))
	{
		qWarning() << "PRAGMA foreign_keys failed:" << q.lastError().text();
	}
	if (!q.exec("PRAGMA journal_mode = WAL;"))
	{
		qWarning() << "PRAGMA journal_mode failed:" << q.lastError().text();
	}
	if (!q.exec("PRAGMA synchronous = NORMAL;"))
	{
		qWarning() << "PRAGMA synchronous failed:" << q.lastError().text();
	}

	return true;
}

QSqlDatabase Db::db()
{
	return QSqlDatabase::database(kConnName);
}