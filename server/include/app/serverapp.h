/*************************************************
  * 描述：
  *
  * File：serverapp.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_SERVERAPP_H
#define TEACHERINVSERVER_SERVERAPP_H

#include <QObject>
#include <QtHttpServer/QtHttpServer>

class ServerApp : public QObject {
	Q_OBJECT
  public:
	explicit ServerApp(QObject* parent = nullptr);

	bool init(const QString& sqlitePath);
	bool listen(quint16 port);

private:
	QHttpServer server_;
};

#endif //TEACHERINVSERVER_SERVERAPP_H