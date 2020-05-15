#pragma once
#include <QString>
#include <vector>
#include <qlist.h>
#include <QMetaType>
#include <qjsonobject.h>

enum EN_ENVIRONMENT_TYPE
{
	kTestEnvironment,
	kFormalEnvironment
};

enum EN_LIVE_ROOM_COVER_TYPE
{
	kCustomPicture,				// 自定义图片
	kLiveFrame				// 直播画面
	
};

struct ST_USER_INFORMATION
{
	QString qsIMUserID;
	QString qsIMUserSignature;
	QString qsPortrait;
	QString qsNickName;
	QString qsLivePushAddress;
	QString qsToken;
	QString qsTQID;
	QString qsRoomNumber;
	int nShowRoomNumber;			// 水印中是否显示房间号
	QString qsRoomName;
	int nRoomID;
	QString qsGroupID;
	int m_nLiveRecordID;						// 直播记录ID
	int nHotValue;
	int nLiveScheduleScope;
	QString qsGeneralizeGroupNumber;
	int nGeneralizeGroupID;
	int nDomainPoolIndex;
	EN_LIVE_ROOM_COVER_TYPE eLiveRoomCoverType;
	QString qsCoverURL;
	QString qsLiveAdvanceNotice;
	ST_USER_INFORMATION()
		: m_nLiveRecordID(0)
		, nShowRoomNumber(0)
	{

	}
};

enum EN_MSG_TYPE
{
	kTextType,
	kImageType
};

Q_DECLARE_METATYPE(EN_MSG_TYPE)

enum EN_DOWN_TYPE
{
	kUserPhoto,
	kIMMsgImage
};

enum EN_CONV_TYPE
{
	kC2CConv = 1,
	kGroupConv

};

enum EN_MSG_LOAD_TYPE
{
	kMsgReload,
	kMsgLoadMore
};

struct ST_FRIEND_INFO
{
	QString qsUserID;
	QString qsNickName;
	QString qsPortrait;

	bool operator == (const ST_FRIEND_INFO &ref)
	{
		if (this->qsUserID == ref.qsUserID)
		{
			return true;
		}

		return false;
	}
};

struct ST_PROHIBIT_TO_SPEAK_RECORD
{
	int nUserID;
	QString qsNickName;
	QString qsOperator;
	QString qsProhibitEndTime;
};

struct ST_ROOM_ADMINISTRATOR_RECORD
{
	int nUserID;
	QString qsNickName;
	QString qsAppointedDateTime;
	QString qsLastEntryRoomDateTime;
};

struct ST_VER_INFO
{
	QString qsVersion;
	QString qsDownloadURL;
	QString qsUpdateContent;
};

struct ST_LIVE_CONTENT
{
	QString qsCategoryName;
	int nCategoryID;
	QString qsClassifyName;
	int nClassifyID;
};

struct ST_LIVE_CATEGORY_INFO
{
	QString qsCategoryName;
	int nCategoryID;
};

struct ST_LIVE_STATISTICS
{
	int nSupremeHotValue;
	int nNewlyAddedAttentionValue;
	int nNewlyAddedGroupNumValue;
	int nReveivedGifts;					// 探球币
	int nBeanGifts;						// 探球豆
	QString qsLiveDuration;
};

struct ST_OSS_INFORMATION
{
	QString qsAccessKeyId;
	QString qsAccessKeySecret;
	QString qsEndPoint;
	QString qsSecurityToken;
	QString qsBucket;
	QString qsCatalog;
	QString qsPrefix;
};

struct ST_SYSTEM_ANNOUNCEMENT
{
	QString qsTitle;
	QString qsURL;
};

struct ST_GROUP_INFO
{
	QString qsGroupName;
	QString qsGroupNumber;
	int nGroupID;
	int nGroupNumOfPeople;
	int nOnlineGroupNumOfPeople;
};

struct ST_MATCH_INFO
{
	bool bIsScheduleData;
	int nCategoryID;
	int nMatchID;
	QString qsDateTime;
	QString qsCompetitionName;
	QString qsContent;
	QString qsHostTeam;
	QString qsGuestTeam;
	QString qsMatchTime;
	int nScheduleRecordID;

	ST_MATCH_INFO()
		: bIsScheduleData(false)
	{

	}

	QString operator[](int index) const
	{
		switch (index)
		{
		case 0:
			return qsDateTime;
		case 1:
			return qsCompetitionName;
		case 2:
			return qsContent;
		case 4:
			return qsHostTeam;
		case 5:
			return qsGuestTeam;
		default:
			return QString("");
		}
	}
};

struct ST_CONV_INFO
{
	QString qsConvID;
	QString qsConvName;
	EN_CONV_TYPE eConvType;
	EN_MSG_TYPE eMsgType;
	QString qsLatestMsgInfo;						// 最新消息
	bool bHasLastMsg;
	QString qsConvPortrait;
	QJsonObject jsonStartMsg;						// 当前显示的起始消息
	bool operator == (const ST_CONV_INFO &ref)
	{
		if (this->qsConvID == ref.qsConvID)
		{
			return true;
		}

		return false;
	}
};

struct ST_MSG_INFO
{
	QString qsConvID;
	EN_MSG_TYPE eMsgType;
	bool isFromMe;
	QString qsSendTime;
	QString qsMsgInfo;
};

struct ST_TEXT_MSG_INFO : public ST_MSG_INFO
{
	QString qsTextContent;
};

struct ST_IMAGE_MSG_INFO : public ST_MSG_INFO
{
	QString qsLocalFilePath;
};

struct ST_IM_MSG_INFO
{
	EN_MSG_TYPE eMsgType;
	QString qsData;
};

struct ST_COMPETITION_INFO
{
	int nCompetitionID;
	QString qsCompetitionName;
};

enum EN_LOGIN_METHOD
{
	kPhoneMethod,
	kAccountMethod
};

typedef struct ST_ACCOUNT_INFORMATION
{
	QString qsAccountNumber;
	QString qsPassword;
	bool isRememberPwd;
	EN_LOGIN_METHOD eLoginMethod;
	bool autoLoginMark;

	ST_ACCOUNT_INFORMATION()
	{
		isRememberPwd = false;
		autoLoginMark = false;
	}

	bool operator == (const ST_ACCOUNT_INFORMATION &stAccountInfo)
	{
		if (eLoginMethod == stAccountInfo.eLoginMethod)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
} STAccountInformation;

class CDataCentre
{
public:
	CDataCentre();
	~CDataCentre();
	void InitConfiguration();
public:
	EN_ENVIRONMENT_TYPE m_eEnvironmentType;
	QString m_qsServerAddress;
	unsigned long long m_ulAppID;
	ST_USER_INFORMATION stUserInformation;
	QList<ST_FRIEND_INFO> m_obLFriendInfo;
	QList<ST_CONV_INFO> m_obLConvInfo;
	QList<ST_COMPETITION_INFO> m_obLCompetitionList;
	ST_ACCOUNT_INFORMATION m_stCurrentAccountInfo;
};

extern CDataCentre g_dataCentre;

