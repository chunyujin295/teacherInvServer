/*************************************************
  * 描述：
  *
  * File：httpjson.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_HTTPJSON_H
#define TEACHERINVSERVER_HTTPJSON_H

#include <QtHttpServer/QHttpServerResponse>
#include <QJsonObject>
#include <QJsonArray>

namespace HttpJson
{
	QHttpServerResponse ok(const QJsonObject& data);
	QHttpServerResponse badRequest(const QString& message, const QJsonObject& detail = {});
	QHttpServerResponse unauthorized(const QString& message);
	QHttpServerResponse notFound(const QString& message);
	QHttpServerResponse serverError(const QString& message);
}


#endif //TEACHERINVSERVER_HTTPJSON_H