/*************************************************
  * 描述：
  *
  * File：exam.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_EXAM_H
#define TEACHERINVSERVER_EXAM_H

#include <QString>

struct Exam
{
	QString proctoringID;// 主键
	QString proctoringName;
	QString startTime;// 先沿用你原来的字符串格式：2023-11-20#14:30
	QString endTime;
	int allTimeMinutes = 0;// 分钟
	QString access;
};

#endif //TEACHERINVSERVER_EXAM_H