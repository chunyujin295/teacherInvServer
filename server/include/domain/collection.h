/*************************************************
  * 描述：
  *
  * File：collection.h
  * Date：2026/1/18
  * Update：
  * ************************************************/
#ifndef TEACHERINVSERVER_COLLECTION_H
#define TEACHERINVSERVER_COLLECTION_H

#include <QString>

struct CollectionRow {
	QString proctoringID;
	QString proctoringName;
	QString startTime;
	QString endTime;
	int allTimeMinutes = 0;
	QString access;
	QString teacherID;
	QString teacherName;
};

#endif //TEACHERINVSERVER_COLLECTION_H