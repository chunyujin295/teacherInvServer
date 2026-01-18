/*************************************************
  * 描述：监考信息类
  *
  * File：invinformation.hpp
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_INVINFORMATION_HPP
#define TEACHERINVSERVER_INVINFORMATION_HPP
#include <QList>
#include <QString>

namespace InvServer
{
	class InvInformation// 监考信息
	{

	public:
		InvInformation(const QString& proctoringID,
		               const QString& teacherID)
		{
			this->proctoringID = proctoringID;
			this->teacherID = teacherID;
		}

		QList<QString> getInformation() const;

	private:
		QString proctoringID;
		QString teacherID;
	};

	inline QList<QString> InvInformation::getInformation() const
	{
		QList<QString> allInformation;
		allInformation.push_back(proctoringID);
		allInformation.push_back(teacherID);
		return allInformation;
	}

}

#endif //TEACHERINVSERVER_INVINFORMATION_HPP