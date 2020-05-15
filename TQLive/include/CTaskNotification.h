#pragma once

#include <QObject>
#include "CDataCentre.h"

class CTaskNotification : public QObject
{
	Q_OBJECT

public:
	CTaskNotification(QObject *parent);
	~CTaskNotification();
	static CTaskNotification *CTaskNotification::GetInstance();

private:
	static CTaskNotification *m_pInstance;

signals:
	void signalLoginTaskNotification(int, QString);
	void signalFinishLiveTaskNotification(int, QString);
	void signalLogoutTaskNotification(int, QString);
	void signalHeartbeatValidationTaskNotification(int, QString);
	void signalLiveRoomSetupTaskNotification(int, QString);
	void signalIMLogin();
	void signalRecvChatRoomMsg(const QString &);
	void signalAcquireLivePushAddress(int, const QString &);
	void signalAcquireCompetitionList(int, const QString &, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData);
	void signalAcquireMatchList(int, const QString &, QList<ST_MATCH_INFO> obLMatchList, const void *userData);
	void signalAcquireLiveScheduleList(int, const QString &, QList<ST_MATCH_INFO>);

	void signalAcquireFriendList(int nStatuCode, const QString &qsInfo, const void *userData = NULL);
	void signalDownloadFileFinish(int nCode, const QString &qsInfo, const void *receiver = NULL, const void *userData = NULL);
	void signalDownloadFileNotification(int nCode, const QString &qsInfo, const void *userData);
	//void signalReceiveIMImageMsg(int nCode, const QString &qsInfo, const void *userData);
	//void signalIMRecvImageMsg(QString qsUserID, QString qsLocalPath);
	//void signalIMRecvTextMsg(const QString &qsUserID, const QString &qsTextPath);

	void signalIMRecvImageMsg(const ST_MSG_INFO &stMsgInfo);
	void signalIMRecvTextMsg(const ST_MSG_INFO &stMsgInfo);

	void signalIMAcquireConv();
	void signalIMAcquireConvList();
	void signalAcquireConvMsgList(const QList<ST_MSG_INFO> &obLMsgInfo, const void *);
	void signalIMSendMsg();
	void signalIMRecvMsg(EN_MSG_TYPE eMsgType, void *userData);

	void signalIMLogout();
	void signalJSExecuted(const QVariant &v);

	void signalUserPortraitDownloadFinish();

	// 禁言记录列表
	void signalAcquireProhibitToSpeakList(int nStatuCode, const QString &qsInfo, QList<ST_PROHIBIT_TO_SPEAK_RECORD> obList);

	// 房间管理员列表
	void signalAcquireRoomAdministratorList(int, const QString &, QList<ST_ROOM_ADMINISTRATOR_RECORD>);

	// 推广群列表
	void signalAcquireGroupGeneralizeList(int, const QString &, QList<ST_GROUP_INFO>);

	// 取消用户禁言
	void signalRepealSilent(int, const QString &);

	// 取消房间管理员
	void signalRepealRoomAdministrator(int, const QString &);

	// 获取聊天室信息
	void signalIMAcquireGroupInfo(int nStatuCode, const QString &qsMsg, const ST_GROUP_INFO &stGroupInfo);

	// 开播提醒任务通知
	void signalSendStartToBroadcastReminder(int nStatusCode, const QString &qsMsg);

	// 结束直播统计信息任务通知
	void signalAcquireLiveEndStatistics(int nStatusCode, const QString &qsMsg, const ST_LIVE_STATISTICS &stLiveStatistics, const void *userData);

	// 获取系统公告
	void signalAcquireSystemAnnouncementList(int, const QString &, const QList<ST_SYSTEM_ANNOUNCEMENT> &obList, const void *userData);

	// 获取直播提醒内容与剩余发送次数
	void signalAcquireReminderContent(int nStatusCode, const QString &qsMsg, int nRemainedTime, const void *userData);

	// 获取直播分类列表
	void signalAcquireLiveCategory(int nStatusCode, const QString &qsMsg, const QList<ST_LIVE_CATEGORY_INFO> &obList, const void *userData);

	// 获取服务器端最新版本
	void signalAcquireAppUpToDate(int nStatusCode, const QString &qsMsg, const ST_VER_INFO &stVerInfo, const void *userData);

	// 更新直播内容通知
	void signalUpdateLiveContentTaskNotification(int, QString, const void *userData);

	// 获取默认的直播内容
	void signalAcquireDefaultLiveContent(int nStatusCode, const QString &qsMsg, const ST_LIVE_CONTENT &stLiveContent, const void *userData);

	// 获取OSS相关信息
	void signalAcquireOSSInformation(int nStatusCode, const QString &qsMsg, const ST_OSS_INFORMATION &stOSSInformation, const void *userData);

	// 上传文件
	void signalUploadFile(int nStatusCode, const QString &qsMsg, const QString &qsFileID, const void *userData);
};
