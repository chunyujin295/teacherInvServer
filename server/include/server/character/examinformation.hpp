/*************************************************
  * 描述：考试信息类定义
  *
  * File：examinformation.hpp
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_EXAMINFORMATION_HPP
#define TEACHERINVSERVER_EXAMINFORMATION_HPP
#include <QList>
#include <QString>


namespace InvServer
{
	class ExamInformation// 考试信息
	{
		friend class System;// 设置ProctoringSystem为友元类，使其可以访问Information的私有成员
	public:
		ExamInformation(const QString& proctoringID,
		                 const QString& proctoringName,
		                 const QString& startTime,
		                 const QString& endTime,
		                 const QString& allTime,
		                 const QString& access)
		{
			this->proctoringID = proctoringID;
			this->proctoringName = proctoringName;
			this->startTime = startTime;
			this->endTime = endTime;
			this->allTime = allTime;
			this->access = access;
		}

		QList<QString> getInformation()
		{
			QList<QString> all_Exam_Information;
			all_Exam_Information.push_back(proctoringID);
			all_Exam_Information.push_back(proctoringName);
			all_Exam_Information.push_back(startTime);
			all_Exam_Information.push_back(endTime);
			all_Exam_Information.push_back(allTime);
			all_Exam_Information.push_back(access);
			return all_Exam_Information;
		}

	private:
		QString proctoringID;// 考试编号，主键
		QString proctoringName;
		QString startTime;// 考试开始时间，格式2023-11-20#14:30
		QString endTime;// 考试结束时间
		QString allTime;// 考试总时长，单位为分钟
		QString access;// 考试地点
	};
}

#endif //TEACHERINVSERVER_EXAMINFORMATION_HPP