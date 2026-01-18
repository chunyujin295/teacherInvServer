/*************************************************
  * 描述：
  *
  * File：proctoringrepo.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_PROCTORINGREPO_H
#define TEACHERINVSERVER_PROCTORINGREPO_H

#include <QList>
#include <QString>
#include <optional>

#include <domain/proctoring.h>
#include <domain/apierror.h>
#include <repo/teacherrepo.h> // for RepoResult/RepoResultVoid templates

struct ProctoringFilter
{
	QString proctoringID;
	QString teacherID;
};

class ProctoringRepo
{
public:
	RepoResult<bool> exists(const QString& proctoringId, const QString& teacherId) const;

	RepoResult<QList<Proctoring>> listAll() const;
	RepoResult<QList<Proctoring>> search(const ProctoringFilter& f) const;

	RepoResult<QList<QString>> listProctoringIdsByTeacher(const QString& teacherId) const;

	RepoResultVoid insert(const Proctoring& p) const;
	RepoResultVoid deleteOne(const QString& proctoringId, const QString& teacherId) const;
};


#endif //TEACHERINVSERVER_PROCTORINGREPO_H