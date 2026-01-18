/*************************************************
  * 描述：
  *
  * File：db.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_DB_H
#define TEACHERINVSERVER_DB_H

#include <QString>
#include <QtSql/QSqlDatabase>

class Db
{
public:
	static bool initSqlite(const QString& sqlitePath);
	static QSqlDatabase db();// 默认连接
};

#endif //TEACHERINVSERVER_DB_H