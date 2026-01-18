#include <services/teacherservice.h>
#include <QSqlQuery>
#include <QSqlError>
#include <repo/db.h>
#include <domain/fieldmaps.h>

static ApiError dbErr(const QSqlQuery& q, const QString& where)
{
	ApiError e;
	e.code = "db_error";
	e.message = "Database query failed";
	e.detail.insert("where", where);
	e.detail.insert("qt_error", q.lastError().text());
	return e;
}

ServiceJsonResult TeacherService::teacherExamInformationSelect(const QString& teacherId) const
{
	ServiceJsonResult r;

	// 旧逻辑：exam_information WHERE proctoringID IN (SELECT proctoringID FROM proctoring_information WHERE teacherID=?)
	// 新逻辑：JOIN 一步查，性能更好
	QSqlQuery q(Db::db());
	q.prepare(R"SQL(
    SELECT e.proctoringID, e.proctoringName, e.startTime, e.endTime, e.allTime, e.access
    FROM proctoring_information p
    JOIN exam_information e ON e.proctoringID = p.proctoringID
    WHERE p.teacherID = :tid
    ORDER BY e.proctoringID
  )SQL");
	q.bindValue(":tid", teacherId);

	if (!q.exec())
	{
		r.ok = false;
		r.error = dbErr(q, "TeacherService::teacherExamInformationSelect");
		return r;
	}

	// 兼容你旧 JSON key：a_proctoringID...f_access
	while (q.next())
	{
		QJsonObject obj;
		obj.insert(FieldMaps::Exam_OutKeys[0], q.value(0).toString());
		obj.insert(FieldMaps::Exam_OutKeys[1], q.value(1).toString());
		obj.insert(FieldMaps::Exam_OutKeys[2], q.value(2).toString());
		obj.insert(FieldMaps::Exam_OutKeys[3], q.value(3).toString());
		obj.insert(FieldMaps::Exam_OutKeys[4], q.value(4).toInt());
		obj.insert(FieldMaps::Exam_OutKeys[5], q.value(5).toString());
		r.data.push_back(obj);
	}

	return r;
}