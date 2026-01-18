/*************************************************
  * 描述：
  *
  * File：rootrepo.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_ROOTREPO_H
#define TEACHERINVSERVER_ROOTREPO_H

#include <QString>
#include <optional>

#include <domain/apierror.h>
#include <repo/teacherrepo.h> // 复用 RepoResult/RepoResultVoid

class RootRepo
{
public:
	// root 表结构：root(rootID TEXT PRIMARY KEY, passwd TEXT NOT NULL)
	RepoResult<bool> verifyPassword(const QString& rootId, const QString& passwd) const;
};

#endif //TEACHERINVSERVER_ROOTREPO_H