/*************************************************
  * 描述：数据库维护类
  *
  * File：dbinit.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_DBINIT_H
#define TEACHERINVSERVER_DBINIT_H
#include <QString>

namespace ff
{
	class SimpleDB;
}

namespace InvServer
{
	class DbInit
	{
	public:
		explicit DbInit(ff::SimpleDB& db);

		bool init();
		QString lastError() const;

	private:
		bool tableExists(const QString& tableName);

		bool createTeacher();
		bool createExamInformation();
		bool createInvInformation();
		bool createRoot();

	private:
		ff::SimpleDB& m_db;
		QString m_lastError;
	};
}

#endif //TEACHERINVSERVER_DBINIT_H