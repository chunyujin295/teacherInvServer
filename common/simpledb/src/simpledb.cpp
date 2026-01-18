#include <simpledb/simpledb.h>

using namespace ff;

SimpleDB::~SimpleDB()
{
	close();
}

bool SimpleDB::open(const QString& dbPath)
{
	if (QSqlDatabase::contains("simple_connection"))
	{
		m_db = QSqlDatabase::database("simple_connection");
	}
	else
	{
		m_db = QSqlDatabase::addDatabase("QSQLITE", "simple_connection");
		m_db.setDatabaseName(dbPath);
	}

	if (!m_db.open())
	{
		m_lastError = m_db.lastError().text();
		return false;
	}

	// 小课设强烈建议加这两句
	QSqlQuery q(m_db);
	q.exec("PRAGMA foreign_keys = ON");
	q.exec("PRAGMA busy_timeout = 2000");

	return true;
}

void SimpleDB::close()
{
	if (m_db.isValid() && m_db.isOpen())
	{
		m_db.close();
	}
}

bool SimpleDB::exec(const QString& sql)
{
	QSqlQuery q(m_db);
	if (!q.exec(sql))
	{
		m_lastError = q.lastError().text();
		return false;
	}
	return true;
}

QSqlQuery SimpleDB::query(const QString& sql)
{
	QSqlQuery q(m_db);
	if (!q.exec(sql))
	{
		m_lastError = q.lastError().text();
	}
	return q;
}

QString SimpleDB::lastError() const
{
	return m_lastError;
}