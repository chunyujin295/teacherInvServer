/*************************************************
  * 描述：
  *
  * File：fieldmaps.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_FIELDMAPS_H
#define TEACHERINVSERVER_FIELDMAPS_H

#include <QStringList>

namespace FieldMaps
{

	inline const QStringList Exam_OutKeys = {
			"a_proctoringID", "b_proctoringName", "c_startTime", "d_endTime", "e_allTime", "f_access"
	};

	inline const QStringList Exam_DbCols = {
			"proctoringID", "proctoringName", "startTime", "endTime", "allTime", "access"
	};

	inline const QStringList Proctoring_OutKeys = {
			"a_proctoringID", "b_teacherID"
	};

	inline const QStringList Proctoring_DbCols = {
			"proctoringID", "teacherID"
	};

	inline const QStringList Teacher_OutKeys = {
			"a_teacherID", "b_teacherName"
	};

	inline const QStringList Teacher_DbCols = {
			"teacherID", "teacherName"
	};

	inline const QStringList Collection_OutKeys = {
			"a_proctoringID", "b_proctoringName", "c_startTime", "d_endTime",
			"e_allTime", "f_access", "g_teacherID", "h_teacherName"
	};

	inline const QStringList Collection_DbCols = {
			"proctoringID", "proctoringName", "startTime", "endTime",
			"allTime", "access", "teacherID", "teacherName"
	};

}

#endif //TEACHERINVSERVER_FIELDMAPS_H