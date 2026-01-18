/*************************************************
  * 描述：
  *
  * File：teacherservice.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_TEACHERSERVICE_H
#define TEACHERINVSERVER_TEACHERSERVICE_H

#pragma once
#include <QString>
#include <QJsonArray>
#include <optional>

#include <domain/apierror.h>
#include <repo/teacherrepo.h>

struct ServiceJsonResult
{
	bool ok = true;
	std::optional<ApiError> error;
	QJsonArray data;// 返回数组，空数组表示“查询成功但为空”
};

class TeacherService
{
public:
	ServiceJsonResult teacherExamInformationSelect(const QString& teacherId) const;
};


#endif //TEACHERINVSERVER_TEACHERSERVICE_H