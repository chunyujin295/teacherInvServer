#include <repo/collectionrepo.h>
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

static CollectionRow rowToCollection(const QSqlQuery& q)
{
	CollectionRow r;
	r.proctoringID = q.value(0).toString();
	r.proctoringName = q.value(1).toString();
	r.startTime = q.value(2).toString();
	r.endTime = q.value(3).toString();
	r.allTimeMinutes = q.value(4).toInt();
	r.access = q.value(5).toString();
	r.teacherID = q.value(6).toString();
	r.teacherName = q.value(7).toString();
	return r;
}

RepoResult<QList<CollectionRow>> CollectionRepo::listAll() const
{
	RepoResult<QList<CollectionRow>> r;
	QList<CollectionRow> out;

	QSqlQuery q(Db::db());
	q.prepare(R"SQL(
    SELECT e.proctoringID, e.proctoringName, e.startTime, e.endTime, e.allTime, e.access,
           t.teacherID, t.teacherName
    FROM proctoring_information p
    JOIN exam_information e ON e.proctoringID = p.proctoringID
    JOIN teacher t ON t.teacherID = p.teacherID
    ORDER BY e.proctoringID, t.teacherID
  )SQL");

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "CollectionRepo::listAll");
		return r;
	}

	while (q.next()) out.push_back(rowToCollection(q));
	r.value = out;
	return r;
}

RepoResult<QList<CollectionRow>> CollectionRepo::search(const CollectionFilter& f) const
{
	RepoResult<QList<CollectionRow>> r;
	QList<CollectionRow> out;

	QString sql = R"SQL(
    SELECT e.proctoringID, e.proctoringName, e.startTime, e.endTime, e.allTime, e.access,
           t.teacherID, t.teacherName
    FROM proctoring_information p
    JOIN exam_information e ON e.proctoringID = p.proctoringID
    JOIN teacher t ON t.teacherID = p.teacherID
  )SQL";

	QStringList where;
	if (!f.proctoringID.isEmpty()) where << "e.proctoringID = :pid";
	if (!f.proctoringName.isEmpty()) where << "e.proctoringName = :pname";
	if (!f.startTime.isEmpty()) where << "e.startTime = :st";
	if (!f.endTime.isEmpty()) where << "e.endTime = :et";
	if (f.allTimeMinutes.has_value()) where << "e.allTime = :allTime";
	if (!f.access.isEmpty()) where << "e.access = :acc";
	if (!f.teacherID.isEmpty()) where << "t.teacherID = :tid";
	if (!f.teacherName.isEmpty()) where << "t.teacherName = :tname";

	if (!where.isEmpty()) sql += " WHERE " + where.join(" AND ");
	sql += " ORDER BY e.proctoringID, t.teacherID";

	QSqlQuery q(Db::db());
	q.prepare(sql);

	if (!f.proctoringID.isEmpty()) q.bindValue(":pid", f.proctoringID);
	if (!f.proctoringName.isEmpty()) q.bindValue(":pname", f.proctoringName);
	if (!f.startTime.isEmpty()) q.bindValue(":st", f.startTime);
	if (!f.endTime.isEmpty()) q.bindValue(":et", f.endTime);
	if (f.allTimeMinutes.has_value()) q.bindValue(":allTime", *f.allTimeMinutes);
	if (!f.access.isEmpty()) q.bindValue(":acc", f.access);
	if (!f.teacherID.isEmpty()) q.bindValue(":tid", f.teacherID);
	if (!f.teacherName.isEmpty()) q.bindValue(":tname", f.teacherName);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "CollectionRepo::search");
		return r;
	}

	while (q.next()) out.push_back(rowToCollection(q));
	r.value = out;
	return r;
}