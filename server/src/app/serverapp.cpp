#include <app/serverapp.h>
#include <QHostAddress>
#include <QDebug>

#include <repo/Db.h>
#include <infra/Migrations.h>
#include <http/Routes.h>

ServerApp::ServerApp(QObject* parent) : QObject(parent) {}

bool ServerApp::init(const QString& sqlitePath) {
	if (!Db::initSqlite(sqlitePath)) {
		qCritical() << "DB init failed.";
		return false;
	}
	if (!Migrations::apply()) {
		qCritical() << "Migrations failed.";
		return false;
	}

	Routes::registerAll(server_);
	return true;
}

bool ServerApp::listen(quint16 port) {
	const auto res = server_.listen(QHostAddress::Any, port);
	if (!res) {
		qCritical() << "Listen failed on port" << port;
		return false;
	}
	qInfo() << "Listening on port" << port;
	return true;
}
