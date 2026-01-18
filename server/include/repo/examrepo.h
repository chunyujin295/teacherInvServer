/*************************************************
  * 描述：
  *
  * File：examrepo.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_EXAMREPO_H
#define TEACHERINVSERVER_EXAMREPO_H

#include <QList>
#include <QString>
#include <QMap>
#include <optional>

#include <domain/exam.h>
#include <domain/apierror.h>
#include <repo/teacherrepo.h> // for RepoResult/RepoResultVoid templates

// 过滤器：哪个字段有值就加 WHERE 条件（AND）
// 用真实列名：proctoringID/proctoringName/startTime/endTime/allTime/access
struct ExamFilter
{
	QString proctoringID;
	QString proctoringName;
	QString startTime;
	QString endTime;
	std::optional<int> allTimeMinutes;
	QString access;
};

class ExamRepo
{
public:
	RepoResult<Exam> findById(const QString& proctoringId) const;
	RepoResult<QList<Exam>> listAll() const;
	RepoResult<QList<Exam>> search(const ExamFilter& f) const;

	RepoResultVoid insert(const Exam& e) const;
	RepoResultVoid updateById(const QString& proctoringId, const Exam& patch) const;// patch字段允许部分为空/缺省
	RepoResultVoid deleteById(const QString& proctoringId) const;

	RepoResult<bool> exists(const QString& proctoringId) const;
};


#endif //TEACHERINVSERVER_EXAMREPO_H