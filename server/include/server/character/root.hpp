/*************************************************
  * 描述：管理员类定义
  *
  * File：root.hpp
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_ROOT_HPP
#define TEACHERINVSERVER_ROOT_HPP

#include <QString>

namespace InvServer
{
	class Root
	{
	public:
		Root(QString username, QString passwd)
		{
			this->username = username;
			this->passwd = passwd;
		}

	private:
		QString username;// 主键
		QString passwd;
	};

}

#endif //TEACHERINVSERVER_ROOT_HPP