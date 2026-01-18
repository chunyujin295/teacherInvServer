#include <server/dbinit/dbinit.h>
#include <QtSql/QSqlQuery>
#include <FFSimpleDB>
using namespace InvServer;

DbInit::DbInit(ff::SimpleDB& db)
	: m_db(db)
{
}

QString DbInit::lastError() const
{
	return m_lastError;
}

bool DbInit::init()
{
	if (!tableExists("Teacher") && !createTeacher()) return false;
	if (!tableExists("ExamInformation") && !createExamInformation()) return false;
	if (!tableExists("InvInformation") && !createInvInformation()) return false;
	if (!tableExists("Root") && !createRoot()) return false;
	return true;
}

bool DbInit::tableExists(const QString& tableName)
{
	QSqlQuery q = m_db.query(
			QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'")
			.arg(tableName)
			);
	return q.next();
}

/* ================== 建表 ================== */

bool DbInit::createTeacher()
{
	const char* sql = R"(
        CREATE TABLE Teacher (
            TeacherId   TEXT PRIMARY KEY,
            TeacherName TEXT
        )
    )";
	if (!m_db.exec(sql))
	{
		m_lastError = m_db.lastError();
		return false;
	}
	return true;
}

bool DbInit::createExamInformation()
{
	const char* sql = R"(
        CREATE TABLE ExamInformation (
            ProctoringId   TEXT PRIMARY KEY,
            ProctoringName TEXT NOT NULL,
            StartTime      TEXT NOT NULL,
            EndTime        TEXT NOT NULL,
            AllTime        TEXT NOT NULL,
            Access         TEXT NOT NULL
        )
    )";
	if (!m_db.exec(sql))
	{
		m_lastError = m_db.lastError();
		return false;
	}
	return true;
}

bool DbInit::createInvInformation()
{
	const char* sql = R"(
        CREATE TABLE InvInformation (
            ProctoringId TEXT NOT NULL,
            TeacherId    TEXT NOT NULL,
            PRIMARY KEY (ProctoringId, TeacherId)
        )
    )";
	if (!m_db.exec(sql))
	{
		m_lastError = m_db.lastError();
		return false;
	}
	return true;
}

bool DbInit::createRoot()
{
	const char* sql = R"(
        CREATE TABLE Root (
            RootId TEXT PRIMARY KEY,
            Passwd TEXT
        )
    )";
	if (!m_db.exec(sql))
	{
		m_lastError = m_db.lastError();
		return false;
	}
	return true;
}