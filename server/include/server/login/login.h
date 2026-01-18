/*************************************************
  * 描述：登陆处理
  *
  * File：login.h
  * Author：ff
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_LOGIN_H
#define TEACHERINVSERVER_LOGIN_H

#include <simpledb/simpledb.h>
#include <FFLogger>

namespace InvServer
{
	class ServerLogin
	{
	public:
		ServerLogin();
		void init();

	private:
		ff::SimpleDB m_db;
	};
}


#endif //TEACHERINVSERVER_LOGIN_H