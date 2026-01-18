/*************************************************
  * 描述：数据库工具实例
  *
  * File：simpledb.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_SIMPLEDB_H
#define TEACHERINVSERVER_SIMPLEDB_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <simpledb/export.h>

namespace ff
{
	class FF_SIMPLEDB_EXPORT SimpleDB
	{
	public:
		SimpleDB() = default;
		~SimpleDB();

		// 打开数据库（第一次用）
		bool open(const QString& dbPath);

		// 关闭数据库（可选）
		void close();

		// 执行不返回结果的 SQL（建表 / 插入 / 更新 / 删除）
		bool exec(const QString& sql);

		// 执行查询（返回 QSqlQuery，外部自己 next()）
		QSqlQuery query(const QString& sql);

		// 最近一次错误
		QString lastError() const;

	private:
		QSqlDatabase m_db;
		QString m_lastError;
	};
}

#endif //TEACHERINVSERVER_SIMPLEDB_H