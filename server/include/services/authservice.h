/*************************************************
  * 描述：登陆服务
  *
  * File：authservice.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_AUTHSERVICE_H
#define TEACHERINVSERVER_AUTHSERVICE_H

#include <QString>
#include <optional>

#include <domain/apierror.h>
#include <repo/teacherrepo.h>
#include <repo/rootrepo.h>

struct AuthResult
{
	bool ok = true;
	std::optional<ApiError> error;
	bool authed = false;
};

class AuthService
{
public:
	AuthResult teacherLogin(const QString& teacherId) const;
	AuthResult rootLogin(const QString& rootId, const QString& passwd) const;

private:
	TeacherRepo teacherRepo_;
	RootRepo rootRepo_;
};


#endif //TEACHERINVSERVER_AUTHSERVICE_H