#pragma once

#include <QObject>
#include "StructData.h"
#include "CDataCentre.h"

class HttpAgent : public QObject
{
	Q_OBJECT

public:
	static HttpAgent* instance();

	/*请求获取禁言用户列表*/
	void RequestOnGetForbiddenUserList();
	/*请求获取房管列表*/
	void RequestOnGetRoomManagerList();
	/*请求获取QQ群列表*/
	void RequestOnGetGroupChatList();
	/*请求取消该userId的禁言用户*/
	void RequestOnCancelForbiddenUser(int userId);
	/*请求删除该userId的房管*/
	void RequestOnDeleteRoomManager(int userId);
	/*请求开播提醒*/
	void RequestOnBroadcastReminder();
	/*请求关播统计信息*/
	void RequestOnOffcastStatistics(int nLiveRecordID);
	/*请求记录用户信息*/
	void RequestOnRecordUserInformation(const QString &qsAccountNumber, const QString &qsPassword, bool isRememberPwd, int nLoginMethod, int nAutoLoginMark);
	/*请求获取开播提醒内容和剩余发送次数*/
	void RequestOnGetReminderContent();
	/*请求获取平台系统公告*/
	void RequestOnGetSystemNotification();
	/*请求修改直播间参数*/
	void RequestOnModifyLiveRoomParams(int nSettingsType, const QString &qsPropertyValue);
	/*请求获取最新版本号*/
	void RequestOnGetAppUpToDateVer();
	/*请求获取聊天室人数*/
	void RequestOnGetChatGroupInfo(const QString &groupNumber);
	/*获取OSS相关信息*/
	void RequestOnGetOSSInfo();
	/*上传本地文件到OSS*/
	void RequestOnUploadFileToOSS(const ST_OSS_INFORMATION &ossInfo, const QString &filePath);
	/*上报水印功能是否开启*/
	void RequestOnWaterMarkFlags(const QString &value);

private slots:
    /*接收弹幕聊天室消息槽函数*/
	void onRecvChatRoomMsg(const QString &recvMsg);

signals:
	/*接收弹幕聊天室消息信号函数*/
	void onSigRecvChatRoomMsg(const BarrageDetialInfo &barrageDetialInfo);
	/*获取禁言用户列表信号函数*/
	void onSigGetForbiddenUserList(int nStatuCode, const QString &qsInfo, QList<ST_PROHIBIT_TO_SPEAK_RECORD> obList);
	/*获取房管列表信号函数*/
	void onSigGetRoomManagerList(int nStatuCode, const QString &qsInfo, QList<ST_ROOM_ADMINISTRATOR_RECORD> recordList);
	/*获取推广群列表信号函数*/
	void onSigGetGroupChatList(int nStatuCode, const QString &qsInfo, QList<ST_GROUP_INFO> groupList);
	/*取消禁言用户信号函数*/
	void onSigCancelForbiddenUser(int nStatuCode, const QString &qsInfo);
	/*取消房管信号函数*/
	void onSigDeleteRoomManager(int nStatuCode, const QString &qsInfo);
	/*开播提醒信号函数*/
	void onSigBroadcastReminder(int nStatusCode, const QString &qsMsg);
	/*关播统计信号函数*/
	void onSigOffcastStatistics(int nStatusCode, const QString &qsMsg, const ST_LIVE_STATISTICS &stLiveStatistics, const void *userData);
	/*获取直播提醒内容与剩余发送次数信号函数*/
	void onSigGetReminderContent(int nStatusCode, const QString &qsMsg, int nRemainedTime, const void *userData);
	/*获取系统公告信号函数*/
	void onSigGetSystemNotification(int nStatusCode, const QString &qsMsg, const QList<ST_SYSTEM_ANNOUNCEMENT> &obList, const void *userData);
	/*获取最新版本号信号函数*/
	void onSigGetAppUpToDateVer(int nStatusCode, const QString &qsMsg, const ST_VER_INFO &stVerInfo, const void *userData);
	/*获取群聊人数信号函数*/
	void onSigGetChatGroupInfo(int nStatusCode, const QString &qsMsg, const ST_GROUP_INFO &stGroupInfo);
	/*IM登录相应信号函数*/
	void onSigIMLoginRespond();
	/*后台警告通知*/
	void onSigWarningTips(int nStatusCode, QString sMsg, int nLimitTime);
	/*获取OSS相关信息信号函数*/
	void onSigGetOSSInfo(int nStatusCode, const QString &sMsg, const ST_OSS_INFORMATION &ossInfo, const void *userData);
	/*上传文件到OSS的信号函数*/
	void onSigUploadFileToOSS(int nStatusCode, const QString &sMsg, const QString &fileID, const void *userData);

private:
	explicit HttpAgent(QObject *parent = 0);
	~HttpAgent();

private:
	/*初始化链接器*/
	void initConnect();
	/*解析聊天室消息体*/
	void ParsingMsg(const QString &elem_data, const QString &elem_ext, const QString &elem_desc);

private:
	static HttpAgent* m_pInstance;
};
