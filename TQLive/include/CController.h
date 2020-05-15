#pragma once
#pragma warning(disable:4819)
class CTaskManager;
#include "CDataCentre.h"

class CController
{
public:
	static CController &GetInstance();
	void InitController();
	void PostInitTask();
	void PostLoginTask(const QString &qsAccountName, const QString &qsPassword, int nAccountType);
	void PostHeartbeatValidationTask(int nBarrageCount, int nGeneralUserNum, int nVIPUserNum, int nRealBarrageCount = 0);
	void PostLogoutTask();

	// 直播间属性设置 1预告 2名称 4封面
	void PostLiveRoomSetupTask(int nSettingsType, const QString &qsPropertyValue);
	void PostLiveRoomNumberSetupTask(const QString &qsValue);

	void AcquireLiveScheduleList();
	void AcquireCompetitionList(int nLiveType, void *userData = NULL);
	void AcquireMatchList(int nLiveType, int nCompetitionID, void *userData = NULL);
	void AcquireLivePushAddress(int nLiveType, int nMatchID, bool bIsScheduleData, int nScheduleRecordID, int nGroupID);
	void PostFinishLiveWorker(const QString &qsRoomNumber);
	void PostDownloadTask(const QString &qsURL, const QString &qsDestPath, void *userData = NULL);
	void PostDownloadTask(const QList<QString> &obLURL, void *userData = NULL);
	void PostDownloadTask(const QString &qsURL, void *receiver = NULL, void *userData = NULL);
	//void PostDownloadTask(const QString &qsURL, const QString &qsDestPath, EN_DOWN_TYPE eDownType);
	void PostQuitTask();
	void QuitIM();
	void RecordUserInformation(const QString &qsAccountNumber, const QString &qsPassword, bool isRememberPwd, int nLoginMethod, int nAutoLoginMark);
	void AcquireUserInformation(QList<STAccountInformation> &obLAccountInformation);
	void PostIMLoginTask();
	void PostIMLogoutTask();
	void PostIMMsgTask(const QString &qsConvID, const QList<ST_MSG_INFO> &obLMsgInfo);
	void AcquireFriendListTask();
	void AcquireGroupInfoTask(const QString &qsGroupNumber);
	void AcquireGroupMemberListTask(const QString &qsGroupNumber);
	void AcquireConvListTask();
	void AcquireConvMsgListTask(const QString &qsConvID, EN_MSG_LOAD_TYPE eMsgLoadType);
	QString AcquireAppTempPath();
	void WaitForIMQuit();
	void SetCrashReport(bool bMark = true);
	void WriteToLogFile(const char *sFormat, ...);

	void AcquireProhibitToSpeakList(void *userData = NULL);
	void AcquireRoomAdministratorList(void *userData = NULL);
	void AcquireGroupGeneralizeList(void *userData = NULL);

	void PostRepealSilent(int nRoomID, int nUserID);
	void PostRepealRoomAdministrator(int nUserID);

	void PostSendStartToBroadcastReminder();
	void AcquireLiveEndStatistics(int nLiveRecordID);

	// 获取开播提醒内容与剩余发送次数
	void AcquireReminderContent();

	// 获取平台系统公告
	void AcquireSystemAnnouncement();

	void AcquireLiveCategoryList();

	void AcquireAppUpToDateVer();

	void PostUpdateLiveContentTask(int nCategoryID, int nMatchID, const void *userData);

	void PostUploadTask(const QString &qsSrcPath, void *userData = NULL);

	void AcquireDefaultLiveContent();

	// 获取OSS相关信息
	void AcquireOSSInformation();

	// 上传本地文件
	void UploadFileTask(const ST_OSS_INFORMATION &stOSSInformation, const QString &qsFilePath);

private:
	CController();
	CController(const CController &rhs);
	
	~CController();
	CController &operator =(const CController &rhs) {};

private:
	static CController controller;
	
};

