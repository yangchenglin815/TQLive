/***********************************************************************************
*								HttpAgent.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 分发和解析网络请求的代理层
***********************************************************************************/
#include "HttpAgent.h"
#include "CTaskNotification.h"
#include "CController.h"

#include <QDebug>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>


HttpAgent* HttpAgent::m_pInstance = NULL;

HttpAgent* HttpAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpAgent;
	}
	return m_pInstance;
}

void HttpAgent::RequestOnGetForbiddenUserList()
{
	CController::GetInstance().AcquireProhibitToSpeakList();
}

void HttpAgent::RequestOnGetRoomManagerList()
{
	CController::GetInstance().AcquireRoomAdministratorList();
}

void HttpAgent::RequestOnGetGroupChatList()
{
	CController::GetInstance().AcquireGroupGeneralizeList();
}

void HttpAgent::RequestOnCancelForbiddenUser(int userId)
{
	CController::GetInstance().PostRepealSilent(g_dataCentre.stUserInformation.nRoomID, userId);
}

void HttpAgent::RequestOnDeleteRoomManager(int userId)
{
	CController::GetInstance().PostRepealRoomAdministrator(userId);
}

void HttpAgent::RequestOnBroadcastReminder()
{
	CController::GetInstance().PostSendStartToBroadcastReminder();
}

void HttpAgent::RequestOnOffcastStatistics(int nLiveRecordID)
{
	CController::GetInstance().AcquireLiveEndStatistics(nLiveRecordID);
}

void HttpAgent::RequestOnRecordUserInformation(const QString & qsAccountNumber, const QString & qsPassword, bool isRememberPwd, int nLoginMethod, int nAutoLoginMark)
{
	CController::GetInstance().RecordUserInformation(qsAccountNumber, qsPassword, isRememberPwd, nLoginMethod, nAutoLoginMark);
}

void HttpAgent::RequestOnGetReminderContent()
{
	CController::GetInstance().AcquireReminderContent();
}

void HttpAgent::RequestOnGetSystemNotification()
{
	CController::GetInstance().AcquireSystemAnnouncement();
}

void HttpAgent::RequestOnModifyLiveRoomParams(int nSettingsType, const QString & qsPropertyValue)
{
	CController::GetInstance().PostLiveRoomSetupTask(nSettingsType, qsPropertyValue);
}

void HttpAgent::RequestOnGetAppUpToDateVer()
{
	CController::GetInstance().AcquireAppUpToDateVer();
}


void HttpAgent::RequestOnGetChatGroupInfo(const QString &groupNumber)
{
	CController::GetInstance().AcquireGroupInfoTask(groupNumber);
}

void HttpAgent::RequestOnGetOSSInfo()
{
	CController::GetInstance().AcquireOSSInformation();
}

void HttpAgent::RequestOnUploadFileToOSS(const ST_OSS_INFORMATION & ossInfo, const QString & filePath)
{
	CController::GetInstance().UploadFileTask(ossInfo, filePath);
}

void HttpAgent::RequestOnWaterMarkFlags(const QString &value)
{
	CController::GetInstance().PostLiveRoomNumberSetupTask(value);
}

void HttpAgent::onRecvChatRoomMsg(const QString &recvMsg)
{
	QByteArray recvMsg_array = recvMsg.toUtf8();

	QJsonParseError jsonErr;
	QJsonDocument document = QJsonDocument::fromJson(recvMsg_array, &jsonErr);
	if (!document.isNull() && jsonErr.error == QJsonParseError::NoError)
	{
		if (document.isArray())
		{
			QJsonArray array = document.array();
			int nSize = array.size();
			for (int i = 0; i < nSize; ++i)
			{
				int message_conv_type = -1;
				int elem_type = -1;
				QJsonObject object = array.at(i).toObject();
				if (object.contains("message_conv_type"))
				{
					QJsonValue value = object.value("message_conv_type");
					if (value.isDouble())
					{
						message_conv_type = value.toInt();
					}
				}

				if (message_conv_type == kTIMConv_Group)
				{
					if (object.contains("message_elem_array"))
					{
						QJsonValue value = object.value("message_elem_array");
						if (value.isArray())
						{
							QJsonArray subArray = value.toArray();
							for (int j = 0; j < subArray.size(); ++j)
							{
								QJsonObject subObject = subArray.at(i).toObject();
								if (subObject.contains("elem_type"))
								{
									QJsonValue value = subObject.value("elem_type");
									if (value.isDouble())
									{
										elem_type = value.toInt();
										if (elem_type == kTIMElem_Custom)
										{
											if (subObject.contains("custom_elem_data"))
											{
												QJsonValue value = subObject.value("custom_elem_data");
												QString elem_ext;
												QString elem_desc;
												QString elem_data = value.toString();
												if (subObject.contains("custom_elem_ext"))
												{
													QJsonValue value = subObject.value("custom_elem_ext");
													if (value.isString())
													{
														elem_ext = value.toString();
													}
												}

												if (subObject.contains("custom_elem_desc"))
												{
													QJsonValue value = subObject.value("custom_elem_desc");
													if (value.isString())
													{
														elem_desc = value.toString();
													}
												}

												ParsingMsg(elem_data, elem_ext, elem_desc);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		qDebug() << jsonErr.error << "---" << jsonErr.errorString();
	}
}

HttpAgent::HttpAgent(QObject *parent)
	: QObject(parent)
{
	initConnect();
}

HttpAgent::~HttpAgent()
{

}

void HttpAgent::initConnect()
{
	connect(CTaskNotification::GetInstance(), SIGNAL(signalRecvChatRoomMsg(const QString &)), this, SLOT(onRecvChatRoomMsg(const QString &)));

	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireProhibitToSpeakList(int, const QString &, QList<ST_PROHIBIT_TO_SPEAK_RECORD>)),
		this, SIGNAL(onSigGetForbiddenUserList(int, const QString &, QList<ST_PROHIBIT_TO_SPEAK_RECORD>)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireRoomAdministratorList(int, const QString &, QList<ST_ROOM_ADMINISTRATOR_RECORD>)),
		this, SIGNAL(onSigGetRoomManagerList(int, const QString &, QList<ST_ROOM_ADMINISTRATOR_RECORD>)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireGroupGeneralizeList(int, const QString &, QList<ST_GROUP_INFO>)),
		this, SIGNAL(onSigGetGroupChatList(int, const QString &, QList<ST_GROUP_INFO>)));

	connect(CTaskNotification::GetInstance(), SIGNAL(signalRepealSilent(int, const QString &)),
		this, SIGNAL(onSigCancelForbiddenUser(int, const QString &)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalRepealRoomAdministrator(int, const QString &)),
		this, SIGNAL(onSigDeleteRoomManager(int, const QString &)));

	connect(CTaskNotification::GetInstance(), SIGNAL(signalSendStartToBroadcastReminder(int, const QString &)), 
		this, SIGNAL(onSigBroadcastReminder(int, const QString &)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireLiveEndStatistics(int, const QString &, const ST_LIVE_STATISTICS &, const void *)),
		this, SIGNAL(onSigOffcastStatistics(int, const QString &, const ST_LIVE_STATISTICS &, const void *)));

	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireReminderContent(int, const QString &, int, const void *)), 
		this, SIGNAL(onSigGetReminderContent(int, const QString &, int, const void *)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireSystemAnnouncementList(int, const QString &, const QList<ST_SYSTEM_ANNOUNCEMENT> &, const void *)), 
		this, SIGNAL(onSigGetSystemNotification(int, const QString &, const QList<ST_SYSTEM_ANNOUNCEMENT> &, const void *)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireAppUpToDate(int, const QString &, const ST_VER_INFO &, const void *)), 
		this, SIGNAL(onSigGetAppUpToDateVer(int, const QString &, const ST_VER_INFO &, const void *)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalIMAcquireGroupInfo(int, const QString &, const ST_GROUP_INFO &)),
		this, SIGNAL(onSigGetChatGroupInfo(int, const QString &, const ST_GROUP_INFO &)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalIMLogin()), this, SIGNAL(onSigIMLoginRespond()));

	connect(CTaskNotification::GetInstance(), SIGNAL(signalAcquireOSSInformation(int, const QString&, const ST_OSS_INFORMATION&, const void*)),
		this, SIGNAL(onSigGetOSSInfo(int, const QString&, const ST_OSS_INFORMATION&, const void*)));
	connect(CTaskNotification::GetInstance(), SIGNAL(signalUploadFile(int, const QString&, const QString&, const void*)),
		this, SIGNAL(onSigUploadFileToOSS(int, const QString&, const QString&, const void*)));
}

void HttpAgent::ParsingMsg(const QString &elem_data, const QString &elem_ext, const QString &elem_desc)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + elem_data);
	BarrageDetialInfo barrageDetialInfo;
	int limitTime = 0;
	QString warningMsg = "";

	switch (elem_ext.toInt())
	{
	case USER_SUSPEND_WARNING:
	case USER_TIPS_WARNING:
	{
		if (!elem_data.isEmpty())
		{
			QScriptValue value = sc.property("stopTime");
			if (!value.isNull())
			{
				limitTime = value.toInt32();
			}
			warningMsg = sc.property("warnMgs").toString();
		}

		emit onSigWarningTips(elem_ext.toInt(), warningMsg, limitTime);
		return;
	}
	case USER_LOGOUT_LIVE:
	{
		barrageDetialInfo.nMsgType = USER_LOGOUT_LIVE;
		barrageDetialInfo.nLeaveTime = QDateTime::currentDateTime().toSecsSinceEpoch();
		break;
	}
	case USER_LOGIN_LIVE:
	{
		barrageDetialInfo.nMsgType = USER_LOGIN_LIVE;
		barrageDetialInfo.nEnterTime = QDateTime::currentDateTime().toSecsSinceEpoch();
		break;
	}
	case USER_SEND_GIFT:
	{
		barrageDetialInfo.nMsgType = USER_SEND_GIFT;
		barrageDetialInfo.sGiftName = sc.property("gift").property("giftName").toString();
		barrageDetialInfo.nGiftNum = sc.property("gift").property("num").toInt32();
		break;
	}
	case USER_SEND_MSG:
	{
		barrageDetialInfo.nMsgType = USER_SEND_MSG;
		break;
	}
	default:
		return;
	}

	barrageDetialInfo.sUserName     = sc.property("user").property("nickname").toString();
	barrageDetialInfo.nUserLevel    = sc.property("user").property("lvUser").toInt32();
	barrageDetialInfo.nUserRole     = sc.property("user").property("role").toInt32();
	barrageDetialInfo.nUserId       = sc.property("user").property("uid").toInt32();
	barrageDetialInfo.nUserVipLevel = sc.property("user").property("lvVip").toInt32();
	barrageDetialInfo.sMsgInfo      = sc.property("msg").toString();

	emit onSigRecvChatRoomMsg(barrageDetialInfo);
}
