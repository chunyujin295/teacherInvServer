/*************************************************
  * 描述：
  *
  * File：apierror.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_APIERROR_H
#define TEACHERINVSERVER_APIERROR_H

#include <QString>
#include <QJsonObject>

struct ApiError
{
	QString code;// e.g. "not_found", "db_error", "bad_request"
	QString message;// 给前端/调用方看的简要说明
	QJsonObject detail;// 可选：字段校验失败细节等
};

#endif //TEACHERINVSERVER_APIERROR_H