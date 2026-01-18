#include <repo/rootrepo.h>
#include <QSqlQuery>
#include <QSqlError>
#include <repo/db.h>

static ApiError dbErr(const QSqlQuery& q, const QString& where)
{
	ApiError e;
	e.code = "db_error";
	e.message = "Database query failed";
	e.detail.insert("where", where);
	e.detail.insert("qt_error", q.lastError().text());
	return e;
}

RepoResult<bool> RootRepo::verifyPassword(const QString& rootId, const QString& passwd) const
{
	RepoResult<bool> r;

	QSqlQuery q(Db::db());
	q.prepare("SELECT 1 FROM root WHERE rootID = :id AND passwd = :pw LIMIT 1");
	q.bindValue(":id", rootId);
	q.bindValue(":pw", passwd);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "RootRepo::verifyPassword");
		return r;
	}

	r.value = q.next();
	return r;
}