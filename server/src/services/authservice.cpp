#include <services/authservice.h>

AuthResult AuthService::teacherLogin(const QString& teacherId) const
{
	AuthResult r;

	const auto ex = teacherRepo_.exists(teacherId);
	if (!ex.ok)
	{
		r.ok = false;
		r.error = ex.error;
		return r;
	}

	r.authed = ex.value.value_or(false);
	return r;
}

AuthResult AuthService::rootLogin(const QString& rootId, const QString& passwd) const
{
	AuthResult r;

	const auto v = rootRepo_.verifyPassword(rootId, passwd);
	if (!v.ok)
	{
		r.ok = false;
		r.error = v.error;
		return r;
	}

	r.authed = v.value.value_or(false);
	return r;
}