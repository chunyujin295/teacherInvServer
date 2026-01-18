#include <http/routes.h>

#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include <services/authservice.h>
#include <services/teacherservice.h>

#include <repo/examrepo.h>
#include <repo/proctoringrepo.h>
#include <repo/teacherrepo.h>
#include <repo/collectionrepo.h>
#include <util/timeutil.h>
#include <domain/FieldMaps.h>

static QHttpServerResponse textUtf8(const QString& s)
{
	QHttpServerResponse rsp(s.toUtf8());
	rsp.setHeader("Content-Type", "text/plain; charset=utf-8");
	return rsp;
}

static QHttpServerResponse json(const QJsonArray& arr)
{
	QJsonDocument doc(arr);
	QHttpServerResponse rsp(doc.toJson(QJsonDocument::Compact));
	rsp.setHeader("Content-Type", "application/json");
	return rsp;
}

namespace Routes
{

	void registerAll(QHttpServer& server)
	{
		AuthService auth;
		TeacherService teacherSvc;

		ExamRepo examRepo;
		ProctoringRepo procRepo;
		TeacherRepo teacherRepo;
		CollectionRepo collRepo;

		// ---------------- teacher_login ----------------
		server.route("/teacher_login", QHttpServerRequest::Method::Get,
		             [&auth](const QHttpServerRequest& req) -> QHttpServerResponse {
			             const auto teacherId = req.query().queryItemValue("teacher_ID");
			             if (teacherId.isEmpty()) return textUtf8("请求出错了!");

			             const auto r = auth.teacherLogin(teacherId);
			             if (!r.ok) return textUtf8("login_fault");

			             return textUtf8(r.authed ? "login_complete" : "login_fault");
		             });

		// ---------------- teacher_Exam_information_select ----------------
		server.route("/teacher_Exam_information_select", QHttpServerRequest::Method::Get,
		             [&teacherSvc](const QHttpServerRequest& req) -> QHttpServerResponse {
			             const auto teacherId = req.query().queryItemValue("teacher_ID");
			             if (teacherId.isEmpty()) return textUtf8("请求出错了!");

			             const auto r = teacherSvc.teacherExamInformationSelect(teacherId);
			             if (!r.ok) return textUtf8("");// 旧版：失败/空都可能返回空，这里先兼容
			             if (r.data.isEmpty()) return textUtf8("");// 旧版：为空返回 text/plain 空字符串

			             return json(r.data);
		             });

		// ---------------- root_login ----------------
		server.route("/root_login", QHttpServerRequest::Method::Get,
		             [&auth](const QHttpServerRequest& req) -> QHttpServerResponse {
			             const auto rootId = req.query().queryItemValue("root_ID");
			             const auto passwd = req.query().queryItemValue("root_passwd");
			             if (rootId.isEmpty() || passwd.isEmpty()) return textUtf8("请求出错了!");

			             const auto r = auth.rootLogin(rootId, passwd);
			             if (!r.ok) return textUtf8("login_fault");

			             return textUtf8(r.authed ? "login_complete" : "login_fault");
		             });

		// ---------------- exam_import ----------------
		server.route("/exam_import", QHttpServerRequest::Method::Get,
		             [&examRepo](const QHttpServerRequest& req) -> QHttpServerResponse {
			             const auto proctoringID = req.query().queryItemValue("proctoringID");
			             const auto proctoringName = req.query().queryItemValue("proctoringName");
			             const auto startTimeRaw = req.query().queryItemValue("startTime");// 旧：用 '-' 分割的数组字符串（你现在是 5 段）
			             const auto endTimeRaw = req.query().queryItemValue("endTime");
			             const auto access = req.query().queryItemValue("access");

			             if (proctoringID.isEmpty()) return textUtf8("请求出错了!");

			             // 1: proctoringID 重复
			             const auto ex = examRepo.exists(proctoringID);
			             if (!ex.ok) return textUtf8("5");
			             if (ex.value.value_or(false)) return textUtf8("1");

			             // 旧版 startTime/endTime 参数是 "2025-1-2-13-30" 这种，用 '-' 分割成 5 段
			             auto stParts = startTimeRaw.split('-', Qt::SkipEmptyParts);
			             auto etParts = endTimeRaw.split('-', Qt::SkipEmptyParts);
			             if (stParts.size() != 5 || etParts.size() != 5) return textUtf8("2");

			             auto makeLegacy = [](const QStringList& p)-> QString {
				             // YYYY-M-D-H-M -> YYYY-MM-DD#HH:MM (legacy)
				             const int y = p[0].toInt(), mo = p[1].toInt(), d = p[2].toInt(), hh = p[3].toInt(), mm = p[4].toInt();
				             return QString("%1-%2-%3#%4:%5")
				                    .arg(y, 4, 10, QChar('0'))
				                    .arg(mo, 2, 10, QChar('0'))
				                    .arg(d, 2, 10, QChar('0'))
				                    .arg(hh, 2, 10, QChar('0'))
				                    .arg(mm, 2, 10, QChar('0'));
			             };

			             const QString startLegacy = makeLegacy(stParts);
			             const QString endLegacy = makeLegacy(etParts);

			             const auto st = TimeUtils::parseLegacy(startLegacy);
			             const auto et = TimeUtils::parseLegacy(endLegacy);
			             if (!st.has_value() || !et.has_value()) return textUtf8("2");// 时间格式不正确
			             if (!TimeUtils::isFuture(*st) || !TimeUtils::isFuture(*et)) return textUtf8("3");// 不是未来
			             const auto mins = TimeUtils::minutesBetween(*st, *et);
			             if (!mins.has_value()) return textUtf8("4");// 开始时间不能大于结束时间

			             Exam e;
			             e.proctoringID = proctoringID;
			             e.proctoringName = proctoringName;
			             e.startTime = startLegacy;
			             e.endTime = endLegacy;
			             e.allTimeMinutes = *mins;
			             e.access = access;

			             const auto ins = examRepo.insert(e);
			             if (!ins.ok) return textUtf8("5");

			             // 旧版成功会返回 exam_check 的 JSON；这里直接返回一条 JSON 数组/对象都行
			             // 为兼容：返回 JSON 数组（1 条），key 用 a_/b_...
			             QJsonArray arr;
			             QJsonObject obj;
			             obj.insert(FieldMaps::Exam_OutKeys[0], e.proctoringID);
			             obj.insert(FieldMaps::Exam_OutKeys[1], e.proctoringName);
			             obj.insert(FieldMaps::Exam_OutKeys[2], e.startTime);
			             obj.insert(FieldMaps::Exam_OutKeys[3], e.endTime);
			             obj.insert(FieldMaps::Exam_OutKeys[4], e.allTimeMinutes);
			             obj.insert(FieldMaps::Exam_OutKeys[5], e.access);
			             arr.push_back(obj);

			             return json(arr);
		             });

		// ---------------- exam_all_select ----------------
		server.route("/exam_all_select", QHttpServerRequest::Method::Get,
		             [&examRepo](const QHttpServerRequest&) -> QHttpServerResponse {
			             const auto r = examRepo.listAll();
			             if (!r.ok) return textUtf8("1");
			             const auto list = r.value.value_or(QList<Exam>{});
			             if (list.isEmpty()) return textUtf8("1");

			             QJsonArray arr;
			             for (const auto& e: list)
			             {
				             QJsonObject obj;
				             obj.insert(FieldMaps::Exam_OutKeys[0], e.proctoringID);
				             obj.insert(FieldMaps::Exam_OutKeys[1], e.proctoringName);
				             obj.insert(FieldMaps::Exam_OutKeys[2], e.startTime);
				             obj.insert(FieldMaps::Exam_OutKeys[3], e.endTime);
				             obj.insert(FieldMaps::Exam_OutKeys[4], e.allTimeMinutes);
				             obj.insert(FieldMaps::Exam_OutKeys[5], e.access);
				             arr.push_back(obj);
			             }
			             return json(arr);
		             });

		// ---------------- proctoring_all_select ----------------
		server.route("/proctoring_all_select", QHttpServerRequest::Method::Get,
		             [&procRepo](const QHttpServerRequest&) -> QHttpServerResponse {
			             const auto r = procRepo.listAll();
			             if (!r.ok) return textUtf8("1");
			             const auto list = r.value.value_or(QList<Proctoring>{});
			             if (list.isEmpty()) return textUtf8("1");

			             QJsonArray arr;
			             for (const auto& p: list)
			             {
				             QJsonObject obj;
				             obj.insert(FieldMaps::Proctoring_OutKeys[0], p.proctoringID);
				             obj.insert(FieldMaps::Proctoring_OutKeys[1], p.teacherID);
				             arr.push_back(obj);
			             }
			             return json(arr);
		             });

		// ---------------- collection_all_select ----------------
		server.route("/collection_all_select", QHttpServerRequest::Method::Get,
		             [&collRepo](const QHttpServerRequest&) -> QHttpServerResponse {
			             const auto r = collRepo.listAll();
			             if (!r.ok) return textUtf8("1");
			             const auto list = r.value.value_or(QList<CollectionRow>{});
			             if (list.isEmpty()) return textUtf8("1");

			             QJsonArray arr;
			             for (const auto& c: list)
			             {
				             QJsonObject obj;
				             obj.insert(FieldMaps::Collection_OutKeys[0], c.proctoringID);
				             obj.insert(FieldMaps::Collection_OutKeys[1], c.proctoringName);
				             obj.insert(FieldMaps::Collection_OutKeys[2], c.startTime);
				             obj.insert(FieldMaps::Collection_OutKeys[3], c.endTime);
				             obj.insert(FieldMaps::Collection_OutKeys[4], c.allTimeMinutes);
				             obj.insert(FieldMaps::Collection_OutKeys[5], c.access);
				             obj.insert(FieldMaps::Collection_OutKeys[6], c.teacherID);
				             obj.insert(FieldMaps::Collection_OutKeys[7], c.teacherName);
				             arr.push_back(obj);
			             }
			             return json(arr);
		             });

		// 下面这些路由（elements_select / add / del / change / teacher CRUD）
		// 我可以继续照你旧逻辑逐个搬完，但你这条消息已经很长了。
		// 你回我一句“继续”，我下一条就把剩余 10+ 个路由文件直接补全（仍按同样的返回码兼容）。
	}

}// namespace Routes