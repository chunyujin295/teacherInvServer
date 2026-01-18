#include <repo/examrepo.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <repo/Db.h>

static ApiError dbErr(const QSqlQuery& q, const QString& where)
{
	ApiError e;
	e.code = "db_error";
	e.message = "Database query failed";
	e.detail.insert("where", where);
	e.detail.insert("qt_error", q.lastError().text());
	return e;
}

static Exam rowToExam(const QSqlQuery& q)
{
	Exam e;
	e.proctoringID = q.value(0).toString();
	e.proctoringName = q.value(1).toString();
	e.startTime = q.value(2).toString();
	e.endTime = q.value(3).toString();
	e.allTimeMinutes = q.value(4).toInt();
	e.access = q.value(5).toString();
	return e;
}

RepoResult<Exam> ExamRepo::findById(const QString& proctoringId) const
{
	RepoResult<Exam> r;

	QSqlQuery q(Db::db());
	q.prepare(R"SQL(
    SELECT proctoringID, proctoringName, startTime, endTime, allTime, access
    FROM exam_information
    WHERE proctoringID = :id
  )SQL");
	q.bindValue(":id", proctoringId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::findById");
		return r;
	}
	if (!q.next())
	{
		r.value = std::nullopt;
		return r;
	}
	r.value = rowToExam(q);
	return r;
}

RepoResult<bool> ExamRepo::exists(const QString& proctoringId) const
{
	RepoResult<bool> r;

	QSqlQuery q(Db::db());
	q.prepare("SELECT 1 FROM exam_information WHERE proctoringID = :id LIMIT 1");
	q.bindValue(":id", proctoringId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::exists");
		return r;
	}
	r.value = q.next();
	return r;
}

RepoResult<QList<Exam>> ExamRepo::listAll() const
{
	RepoResult<QList<Exam>> r;
	QList<Exam> out;

	QSqlQuery q(Db::db());
	q.prepare(R"SQL(
    SELECT proctoringID, proctoringName, startTime, endTime, allTime, access
    FROM exam_information
    ORDER BY proctoringID
  )SQL");

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::listAll");
		return r;
	}

	while (q.next()) out.push_back(rowToExam(q));
	r.value = out;
	return r;
}

RepoResult<QList<Exam>> ExamRepo::search(const ExamFilter& f) const
{
	RepoResult<QList<Exam>> r;
	QList<Exam> out;

	QString sql = R"SQL(
    SELECT proctoringID, proctoringName, startTime, endTime, allTime, access
    FROM exam_information
  )SQL";

	QStringList where;
	if (!f.proctoringID.isEmpty()) where << "proctoringID = :proctoringID";
	if (!f.proctoringName.isEmpty()) where << "proctoringName = :proctoringName";
	if (!f.startTime.isEmpty()) where << "startTime = :startTime";
	if (!f.endTime.isEmpty()) where << "endTime = :endTime";
	if (f.allTimeMinutes.has_value()) where << "allTime = :allTime";
	if (!f.access.isEmpty()) where << "access = :access";

	if (!where.isEmpty()) sql += " WHERE " + where.join(" AND ");
	sql += " ORDER BY proctoringID";

	QSqlQuery q(Db::db());
	q.prepare(sql);

	if (!f.proctoringID.isEmpty()) q.bindValue(":proctoringID", f.proctoringID);
	if (!f.proctoringName.isEmpty()) q.bindValue(":proctoringName", f.proctoringName);
	if (!f.startTime.isEmpty()) q.bindValue(":startTime", f.startTime);
	if (!f.endTime.isEmpty()) q.bindValue(":endTime", f.endTime);
	if (f.allTimeMinutes.has_value()) q.bindValue(":allTime", *f.allTimeMinutes);
	if (!f.access.isEmpty()) q.bindValue(":access", f.access);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::search");
		return r;
	}

	while (q.next()) out.push_back(rowToExam(q));
	r.value = out;
	return r;
}

RepoResultVoid ExamRepo::insert(const Exam& e) const
{
	RepoResultVoid r;

	QSqlQuery q(Db::db());
	q.prepare(R"SQL(
    INSERT INTO exam_information(proctoringID, proctoringName, startTime, endTime, allTime, access)
    VALUES(:id, :name, :start, :end, :allTime, :access)
  )SQL");
	q.bindValue(":id", e.proctoringID);
	q.bindValue(":name", e.proctoringName);
	q.bindValue(":start", e.startTime);
	q.bindValue(":end", e.endTime);
	q.bindValue(":allTime", e.allTimeMinutes);
	q.bindValue(":access", e.access);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::insert");
		return r;
	}
	return r;
}

RepoResultVoid ExamRepo::updateById(const QString& proctoringId, const Exam& patch) const
{
	RepoResultVoid r;

	QStringList sets;
	if (!patch.proctoringID.isEmpty()) sets << "proctoringID = :newId";
	if (!patch.proctoringName.isEmpty()) sets << "proctoringName = :name";
	if (!patch.startTime.isEmpty()) sets << "startTime = :start";
	if (!patch.endTime.isEmpty()) sets << "endTime = :end";
	// allTimeMinutes 0 可能是合法值，所以这里约定：patch.allTimeMinutes<0 表示不改
	// 但我们 struct 默认 0，不好区分。更稳做法：service 层决定是否更新 allTime。
	// 这里提供：如果你想更新 allTime，就传 patch.allTimeMinutes >= 0 且同时显式调用。
	if (patch.allTimeMinutes >= 0) sets << "allTime = :allTime";
	if (!patch.access.isEmpty()) sets << "access = :access";

	if (sets.isEmpty()) return r;

	QSqlQuery q(Db::db());
	q.prepare(QString("UPDATE exam_information SET %1 WHERE proctoringID = :id").arg(sets.join(", ")));
	q.bindValue(":id", proctoringId);

	if (!patch.proctoringID.isEmpty()) q.bindValue(":newId", patch.proctoringID);
	if (!patch.proctoringName.isEmpty()) q.bindValue(":name", patch.proctoringName);
	if (!patch.startTime.isEmpty()) q.bindValue(":start", patch.startTime);
	if (!patch.endTime.isEmpty()) q.bindValue(":end", patch.endTime);
	if (patch.allTimeMinutes >= 0) q.bindValue(":allTime", patch.allTimeMinutes);
	if (!patch.access.isEmpty()) q.bindValue(":access", patch.access);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::updateById");
		return r;
	}
	return r;
}

RepoResultVoid ExamRepo::deleteById(const QString& proctoringId) const
{
	RepoResultVoid r;

	QSqlQuery q(Db::db());
	q.prepare("DELETE FROM exam_information WHERE proctoringID = :id");
	q.bindValue(":id", proctoringId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "ExamRepo::deleteById");
		return r;
	}
	return r;
}