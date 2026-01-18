#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QDebug>

#include <app/serverApp.h>

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("teaInvS_server");
	QCoreApplication::setApplicationVersion("0.1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("Teacher Invigilation Server (Qt6HttpServer + SQLite)");
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption dbOpt({"d", "db"}, "SQLite db file path", "path", "data/teacherinv.db");
	QCommandLineOption portOpt({"p", "port"}, "Listen port", "port", "8081");
	parser.addOption(dbOpt);
	parser.addOption(portOpt);

	parser.process(app);

	const QString dbPath = QDir::cleanPath(parser.value(dbOpt));
	const quint16 port = parser.value(portOpt).toUShort();

	ServerApp server;
	if (!server.init(dbPath)) return 1;
	if (!server.listen(port)) return 2;

	return app.exec();
}