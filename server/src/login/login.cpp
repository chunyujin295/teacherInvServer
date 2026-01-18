//
// Created by 30297 on 2026/1/18.
//

#include <server/login/login.h>

#include <server/dbinit/dbinit.h>

InvServer::ServerLogin::ServerLogin()
{
	this->init();
}

void InvServer::ServerLogin::init()
{
	m_db.open("Teacher.db");

	DbInit dbInit(m_db);
	if (!dbInit.init())
	{
		FF_LOG_ERROR("Teacher.db不存在且生成失败");
	}
	m_db.close();

	m_db.open("ExamInformation.db");
	DbInit dbInit2(m_db);
	if (!dbInit2.init())
	{
		FF_LOG_ERROR("ExamInformation.db不存在且生成失败");
	}
	m_db.close();

	m_db.open("InvInformation.db");
	DbInit dbInit3(m_db);
	if (!dbInit3.init())
	{
		FF_LOG_ERROR("InvInformation.db不存在且生成失败");
	}
	m_db.close();

	m_db.open("Root.db");
	DbInit dbInit4(m_db);
	if (!dbInit4.init())
	{
		FF_LOG_ERROR("Root.db不存在且生成失败");
	}
	m_db.close();

}