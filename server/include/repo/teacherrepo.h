/*************************************************
  * 描述：
  *
  * File：teacherrepo.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_TEACHERREPO_H
#define TEACHERINVSERVER_TEACHERREPO_H
#include <QList>
#include <QString>
#include <optional>

#include <domain/teacher.h>
#include <domain/apierror.h>

struct RepoResultVoid
{
	bool ok = true;
	std::optional<ApiError> error;
};

template<typename T>
struct RepoResult
{
	bool ok = true;
	std::optional<ApiError> error;
	std::optional<T> value;
};

class TeacherRepo
{
public:
	RepoResult<Teacher> findById(const QString& teacherId) const;
	RepoResult<QList<Teacher>> listAll() const;

	RepoResultVoid insert(const Teacher& t) const;
	RepoResultVoid updateById(const QString& teacherId, const Teacher& patch) const;// patch: teacherID/teacherName 允许部分为空
	RepoResultVoid deleteById(const QString& teacherId) const;

	RepoResult<bool> exists(const QString& teacherId) const;
};


#endif //TEACHERINVSERVER_TEACHERREPO_H