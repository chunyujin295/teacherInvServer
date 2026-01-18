/*************************************************
  * 描述：
  *
  * File：collectionrepo.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_COLLECTIONREPO_H
#define TEACHERINVSERVER_COLLECTIONREPO_H

#include <QList>
#include <QString>
#include <optional>

#include <domain/Collection.h>
#include <domain/ApiError.h>
#include <repo/TeacherRepo.h> // for RepoResult/RepoResultVoid templates

struct CollectionFilter
{
	// 这 8 个字段对应你旧 Collection_Information_Name_for_use
	QString proctoringID;
	QString proctoringName;
	QString startTime;
	QString endTime;
	std::optional<int> allTimeMinutes;
	QString access;
	QString teacherID;
	QString teacherName;
};

class CollectionRepo
{
public:
	RepoResult<QList<CollectionRow>> listAll() const;
	RepoResult<QList<CollectionRow>> search(const CollectionFilter& f) const;
};

#endif //TEACHERINVSERVER_COLLECTIONREPO_H