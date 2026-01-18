#include <infra/migrations.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "repo/Db.h"

static bool execOrLog(QSqlQuery& q, const QString& sql)
{
	if (!q.exec(sql))
	{
		qCritical() << "SQL failed:" << sql;
		qCritical() << "Error:" << q.lastError().text();
		return false;
	}
	return true;
}

bool Migrations::apply()
{
	auto database = Db::db();
	if (!database.isOpen()) return false;

	QSqlQuery q(database);

	// root 管理员表（兼容旧逻辑：root(rootID, passwd)）
	if (!execOrLog(q, R"SQL(
    CREATE TABLE IF NOT EXISTS root (
      rootID TEXT PRIMARY KEY,
      passwd TEXT NOT NULL
    );
  )SQL"))
		return false;


	// 三张表：teacher / exam_information / proctoring_information
	// 用外键 + CASCADE，把你旧代码里“删/改主键要同步”那坨逻辑直接干掉。
	if (!execOrLog(q, R"SQL(
    CREATE TABLE IF NOT EXISTS teacher (
      teacherID   TEXT PRIMARY KEY,
      teacherName TEXT NOT NULL
    );
  )SQL"))
		return false;

	if (!execOrLog(q, R"SQL(
    CREATE TABLE IF NOT EXISTS exam_information (
      proctoringID   TEXT PRIMARY KEY,
      proctoringName TEXT NOT NULL,
      startTime      TEXT NOT NULL,
      endTime        TEXT NOT NULL,
      allTime        INTEGER NOT NULL,
      access         TEXT NOT NULL
    );
  )SQL"))
		return false;

	if (!execOrLog(q, R"SQL(
    CREATE TABLE IF NOT EXISTS proctoring_information (
      proctoringID TEXT NOT NULL,
      teacherID    TEXT NOT NULL,
      PRIMARY KEY (proctoringID, teacherID),
      FOREIGN KEY (proctoringID) REFERENCES exam_information(proctoringID)
        ON UPDATE CASCADE ON DELETE CASCADE,
      FOREIGN KEY (teacherID) REFERENCES teacher(teacherID)
        ON UPDATE CASCADE ON DELETE CASCADE
    );
  )SQL"))
		return false;

	return true;
}