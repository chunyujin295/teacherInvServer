#include <repo/proctoringrepo.h>
#include <QSqlQuery>
#include <QSqlError>
#include <repo/db.h>

static ApiError dbErr(const QSqlQuery& q, const QString& where) {
  ApiError e;
  e.code = "db_error";
  e.message = "Database query failed";
  e.detail.insert("where", where);
  e.detail.insert("qt_error", q.lastError().text());
  return e;
}

RepoResult<bool> ProctoringRepo::exists(const QString& proctoringId, const QString& teacherId) const {
  RepoResult<bool> r;

  QSqlQuery q(Db::db());
  q.prepare(R"SQL(
    SELECT 1 FROM proctoring_information
    WHERE proctoringID = :pid AND teacherID = :tid
    LIMIT 1
  )SQL");
  q.bindValue(":pid", proctoringId);
  q.bindValue(":tid", teacherId);

  if (!q.exec()) {
    r.ok = false;
    r.error = dbErr(q, "ProctoringRepo::exists");
    return r;
  }
  r.value = q.next();
  return r;
}

RepoResult<QList<Proctoring>> ProctoringRepo::listAll() const {
  RepoResult<QList<Proctoring>> r;
  QList<Proctoring> out;

  QSqlQuery q(Db::db());
  q.prepare("SELECT proctoringID, teacherID FROM proctoring_information ORDER BY proctoringID, teacherID");

  if (!q.exec()) {
    r.ok = false;
    r.error = dbErr(q, "ProctoringRepo::listAll");
    return r;
  }

  while (q.next()) {
    Proctoring p;
    p.proctoringID = q.value(0).toString();
    p.teacherID = q.value(1).toString();
    out.push_back(p);
  }
  r.value = out;
  return r;
}

RepoResult<QList<Proctoring>> ProctoringRepo::search(const ProctoringFilter& f) const {
  RepoResult<QList<Proctoring>> r;
  QList<Proctoring> out;

  QString sql = "SELECT proctoringID, teacherID FROM proctoring_information";
  QStringList where;
  if (!f.proctoringID.isEmpty()) where << "proctoringID = :pid";
  if (!f.teacherID.isEmpty()) where << "teacherID = :tid";
  if (!where.isEmpty()) sql += " WHERE " + where.join(" AND ");
  sql += " ORDER BY proctoringID, teacherID";

  QSqlQuery q(Db::db());
  q.prepare(sql);
  if (!f.proctoringID.isEmpty()) q.bindValue(":pid", f.proctoringID);
  if (!f.teacherID.isEmpty()) q.bindValue(":tid", f.teacherID);

  if (!q.exec()) {
    r.ok = false;
    r.error = dbErr(q, "ProctoringRepo::search");
    return r;
  }

  while (q.next()) {
    Proctoring p;
    p.proctoringID = q.value(0).toString();
    p.teacherID = q.value(1).toString();
    out.push_back(p);
  }
  r.value = out;
  return r;
}

RepoResult<QList<QString>> ProctoringRepo::listProctoringIdsByTeacher(const QString& teacherId) const {
  RepoResult<QList<QString>> r;
  QList<QString> out;

  QSqlQuery q(Db::db());
  q.prepare("SELECT proctoringID FROM proctoring_information WHERE teacherID = :tid ORDER BY proctoringID");
  q.bindValue(":tid", teacherId);

  if (!q.exec()) {
    r.ok = false;
    r.error = dbErr(q, "ProctoringRepo::listProctoringIdsByTeacher");
    return r;
  }

  while (q.next()) out.push_back(q.value(0).toString());
  r.value = out;
  return r;
}

RepoResultVoid ProctoringRepo::insert(const Proctoring& p) const {
  RepoResultVoid r;

  QSqlQuery q(Db::db());
  q.prepare("INSERT INTO proctoring_information(proctoringID, teacherID) VALUES(:pid, :tid)");
  q.bindValue(":pid", p.proctoringID);
  q.bindValue(":tid", p.teacherID);

  if (!q.exec()) {
    r.ok = false;
    r.error = dbErr(q, "ProctoringRepo::insert");
    return r;
  }
  return r;
}

RepoResultVoid ProctoringRepo::deleteOne(const QString& proctoringId, const QString& teacherId) const {
  RepoResultVoid r;

  QSqlQuery q(Db::db());
  q.prepare("DELETE FROM proctoring_information WHERE proctoringID = :pid AND teacherID = :tid");
  q.bindValue(":pid", proctoringId);
  q.bindValue(":tid", teacherId);

  if (!q.exec()) {
    r.ok = false;
    r.error = dbErr(q, "ProctoringRepo::deleteOne");
    return r;
  }
  return r;
}
