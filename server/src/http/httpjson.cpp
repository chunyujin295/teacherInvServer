#include <http/httpjson.h>

static QHttpServerResponse jsonResp(QHttpServerResponse::StatusCode code, const QJsonObject& obj)
{
	return QHttpServerResponse(obj, code);
}

QHttpServerResponse HttpJson::ok(const QJsonObject& data)
{
	QJsonObject body;
	body["ok"] = true;
	body["data"] = data;
	return jsonResp(QHttpServerResponse::StatusCode::Ok, body);
}

QHttpServerResponse HttpJson::badRequest(const QString& message, const QJsonObject& detail)
{
	QJsonObject err;
	err["code"] = "bad_request";
	err["message"] = message;
	if (!detail.isEmpty()) err["detail"] = detail;

	QJsonObject body;
	body["ok"] = false;
	body["error"] = err;
	return jsonResp(QHttpServerResponse::StatusCode::BadRequest, body);
}

QHttpServerResponse HttpJson::unauthorized(const QString& message)
{
	QJsonObject err;
	err["code"] = "unauthorized";
	err["message"] = message;

	QJsonObject body;
	body["ok"] = false;
	body["error"] = err;
	return jsonResp(QHttpServerResponse::StatusCode::Unauthorized, body);
}

QHttpServerResponse HttpJson::notFound(const QString& message)
{
	QJsonObject err;
	err["code"] = "not_found";
	err["message"] = message;

	QJsonObject body;
	body["ok"] = false;
	body["error"] = err;
	return jsonResp(QHttpServerResponse::StatusCode::NotFound, body);
}

QHttpServerResponse HttpJson::serverError(const QString& message)
{
	QJsonObject err;
	err["code"] = "server_error";
	err["message"] = message;

	QJsonObject body;
	body["ok"] = false;
	body["error"] = err;
	return jsonResp(QHttpServerResponse::StatusCode::InternalServerError, body);
}