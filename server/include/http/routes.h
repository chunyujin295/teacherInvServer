/*************************************************
  * 描述：
  *
  * File：routes.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_ROUTES_H
#define TEACHERINVSERVER_ROUTES_H
#include <QtHttpServer/QtHttpServer>
namespace Routes
{
	void registerAll(QHttpServer& server);
}

#endif //TEACHERINVSERVER_ROUTES_H