#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <repo/teacherrepo.h>
#include <repo/Db.h>
#include <domain/apierror.h>


static ApiError dbErr(const QSqlQuery& q, const QString& where)
{
	ApiError e;
	e.code = "db_error";
	e.message = "Database query failed";
	e.detail.insert("where", where);
	e.detail.insert("qt_error", q.lastError().text());
	return e;
}

RepoResult<Teacher> TeacherRepo::findById(const QString& teacherId) const
{
	RepoResult<Teacher> r;

	QSqlQuery q(Db::db());
	q.prepare("SELECT teacherID, teacherName FROM teacher WHERE teacherID = :id");
	q.bindValue(":id", teacherId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherRepo::findById");
		return r;
	}
	if (!q.next())
	{
		r.value = std::nullopt;
		return r;
	}

	Teacher t;
	t.teacherID = q.value(0).toString();
	t.teacherName = q.value(1).toString();
	r.value = t;
	return r;
}

RepoResult<bool> TeacherRepo::exists(const QString& teacherId) const
{
	RepoResult<bool> r;

	QSqlQuery q(Db::db());
	q.prepare("SELECT 1 FROM teacher WHERE teacherID = :id LIMIT 1");
	q.bindValue(":id", teacherId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherRepo::exists");
		return r;
	}
	r.value = q.next();
	return r;
}

RepoResult<QList<Teacher>> TeacherRepo::listAll() const
{
	RepoResult<QList<Teacher>> r;
	QList<Teacher> out;

	QSqlQuery q(Db::db());
	q.prepare("SELECT teacherID, teacherName FROM teacher ORDER BY teacherID");

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherRepo::listAll");
		return r;
	}

	while (q.next())
	{
		Teacher t;
		t.teacherID = q.value(0).toString();
		t.teacherName = q.value(1).toString();
		out.push_back(t);
	}
	r.value = out;
	return r;
}

RepoResultVoid TeacherRepo::insert(const Teacher& t) const
{
	RepoResultVoid r;

	QSqlQuery q(Db::db());
	q.prepare("INSERT INTO teacher(teacherID, teacherName) VALUES(:id, :name)");
	q.bindValue(":id", t.teacherID);
	q.bindValue(":name", t.teacherName);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherRepo::insert");
		return r;
	}
	return r;
}

RepoResultVoid TeacherRepo::updateById(const QString& teacherId, const Teacher& patch) const
{
	RepoResultVoid r;

	// 允许部分更新：如果 patch.teacherID 为空就不改；patch.teacherName 为空也不改
	QStringList sets;
	if (!patch.teacherID.isEmpty()) sets << "teacherID = :newId";
	if (!patch.teacherName.isEmpty()) sets << "teacherName = :name";

	if (sets.isEmpty()) return r;// 没有要更新的字段，视为 ok

	QSqlQuery q(Db::db());
	q.prepare(QString("UPDATE teacher SET %1 WHERE teacherID = :id").arg(sets.join(", ")));
	q.bindValue(":id", teacherId);
	if (!patch.teacherID.isEmpty()) q.bindValue(":newId", patch.teacherID);
	if (!patch.teacherName.isEmpty()) q.bindValue(":name", patch.teacherName);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherRepo::updateById");
		return r;
	}
	return r;
}

RepoResultVoid TeacherRepo::deleteById(const QString& teacherId) const
{
	RepoResultVoid r;

	QSqlQuery q(Db::db());
	q.prepare("DELETE FROM teacher WHERE teacherID = :id");
	q.bindValue(":id", teacherId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherRepo::deleteById");
		return r;
	}
	return r;
}