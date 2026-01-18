/*************************************************
  * 描述：教师类定义
  *
  * File：teacher.hpp
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_TEACHER_HPP
#define TEACHERINVSERVER_TEACHER_HPP

#include <QList>
#include <QString>

namespace InvServer
{
	class Teacher// 教师
	{
	public:
		Teacher(const QString& teacherID, const QString& teacherName)
		{
			this->teacherID = teacherID;
			this->teacherName = teacherName;
		}


		/**
		 * 获取教师信息
		 * @return
		 */
		QList<QString> getInformation() const
		{
			QList<QString> allInformation;
			allInformation.push_back(teacherID);
			allInformation.push_back(teacherName);
			return allInformation;
		}

	private:
		QString teacherID;
		QString teacherName;
	};
}

#endif //TEACHERINVSERVER_TEACHER_HPP