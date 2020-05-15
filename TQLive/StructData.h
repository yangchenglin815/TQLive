/***********************************************************************************
*								StructData.h
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 通用数据结构
***********************************************************************************/
#pragma once

#include <QVariant>
#include <QStringList>
#include <QColor>
#include "EnumData.h"

static inline QColor color_from_int(long long val)
{
	return QColor(val & 0xff,
		(val >> 8) & 0xff,
		(val >> 16) & 0xff,
		(val >> 24) & 0xff);
}

static inline long long color_to_int(QColor color)
{
	auto shift = [&](unsigned val, int shift)
	{
		return ((val & 0xff) << shift);
	};

	return  shift(color.red(), 0) |
		shift(color.green(), 8) |
		shift(color.blue(), 16) |
		shift(color.alpha(), 24);
}

//弹幕详情信息
struct BarrageDetialInfo
{
	int nMsgType;              //消息类型
	int nUserLevel;            //用户级别
	int nUserRole;             //用户角色  1：主播，2：房管，3：超管，0：未知， -1：机器人
	int nUserId;               //用户ID
	int nUserVipLevel;         //用户Vip等级  

	QString sUserName;         //用户名称
	QString sMsgInfo;          //用户消息

	int nGiftNum;              //礼物数量
	QString sGiftName;         //礼物名称

	qint64 nEnterTime;          //进入直播间时间
	qint64 nLeaveTime;          //离开直播间时间

	BarrageDetialInfo() {
		nMsgType      = -1;
		nUserLevel    = -1;
		nUserRole     = -1;
		nUserId       = -1;
		nUserVipLevel = -1;

		nEnterTime    = 0;
		nLeaveTime    = 0;
	}

	BarrageDetialInfo& operator =(const BarrageDetialInfo& obj)
	{
		if (this != &obj)
		{
			this->nMsgType      = obj.nMsgType;
			this->nUserLevel    = obj.nUserLevel;
			this->nUserRole     = obj.nUserRole;
			this->nUserId       = obj.nUserId;
			this->nUserVipLevel = obj.nUserVipLevel;

			this->sUserName     = obj.sUserName;
			this->sMsgInfo      = obj.sMsgInfo;

			this->nGiftNum      = obj.nGiftNum;
			this->sGiftName     = obj.sGiftName;

			this->nEnterTime    = obj.nEnterTime;
			this->nLeaveTime    = obj.nLeaveTime;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(BarrageDetialInfo)


//弹幕统计信息
struct BarrageStatisticsInfo
{
	int nMsgType;               //消息类型
	int nUserId;                //用户ID
	int nUserVipLevel;          //用户Vip等级
	int nUserRole;              //用户角色  1：主播，2：房管，3：超管，0：未知， -1：机器人
	
	qint64 nEnterTime;          //进入直播间时间
	qint64 nLeaveTime;          //离开直播间时间
	 
	int nUserMsgTotal;          //弹幕消息统计条数
	int nRealUserMsgTotal;      //真实弹幕统计个数
	int nTerminalTotal;         //终端个数
	bool bInRoom;               //是否在直播间

	BarrageStatisticsInfo() {
		nMsgType       = -1;
		nUserId        = -1;
		nUserVipLevel  = -1;
		nUserRole      = 0;

		nEnterTime     = 0;
		nLeaveTime     = 0;

		nUserMsgTotal  = 0;
		nRealUserMsgTotal = 0;
		nTerminalTotal = 0;
		bInRoom = true;
	}

	BarrageStatisticsInfo& operator =(const BarrageStatisticsInfo& obj)
	{
		if (this != &obj)
		{
			this->nMsgType       = obj.nMsgType;
			this->nUserId        = obj.nUserId;
			this->nUserVipLevel  = obj.nUserVipLevel;
			this->nUserRole      = obj.nUserRole;

			this->nEnterTime     = obj.nEnterTime;
			this->nLeaveTime     = obj.nLeaveTime;

			this->nUserMsgTotal  = obj.nUserMsgTotal;
			this->nRealUserMsgTotal = obj.nRealUserMsgTotal;
			this->nTerminalTotal = obj.nTerminalTotal;
			this->bInRoom = obj.bInRoom;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(BarrageStatisticsInfo)

//素材类型数据
struct MaterialTypeData
{
	int     nType;
	QString sName;
	QString sIcon;
	QString sHoverIcon;

	MaterialTypeData() {
		nType = -1;
	}
};
Q_DECLARE_METATYPE(MaterialTypeData)

//列表循环类型数据
struct LoopTypeData
{
	int     nType;
	QString sName;
	QString sIcon;
	QString sHoverIcon;

	LoopTypeData() {
		nType = -1;
	}

	LoopTypeData& operator =(const LoopTypeData& obj)
	{
		if (this != &obj)
		{
			this->nType = obj.nType;
			this->sName = obj.sName;
			this->sIcon = obj.sIcon;
			this->sHoverIcon = obj.sHoverIcon;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(LoopTypeData)

//音效数据
struct SoundEffectTypeData
{
	QString qSoundEffectName;
	QString qHotKey;
	QString qAduioFile;

	SoundEffectTypeData& operator =(const SoundEffectTypeData& obj)
	{
		if (this != &obj)
		{
			this->qSoundEffectName = obj.qSoundEffectName;
			this->qHotKey          = obj.qHotKey;
			this->qAduioFile       = obj.qAduioFile;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(SoundEffectTypeData)

//媒体视频数据
struct MediaSourceData
{
	int nId;
	QString sMediaName;
	QString sMediaPath;

	int    nTotalTimeDuration;
	bool   bActive;

	MediaSourceData() {
		nId = 0;
		nTotalTimeDuration = 0;
		bActive = false;
	}

	MediaSourceData& operator =(const MediaSourceData& obj)
	{
		if (this != &obj)
		{
			this->nId = obj.nId;
			this->sMediaName = obj.sMediaName;
			this->sMediaPath = obj.sMediaPath;
			this->nTotalTimeDuration = obj.nTotalTimeDuration;
			this->bActive = obj.bActive;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(MediaSourceData)

struct SourceIniData
{
	QString sourceName;

	int cycleType;
	int voiceVal;
	int listLength;

	QList<MediaSourceData> mediaSourceList;

	SourceIniData() {
		cycleType  = 2;
		voiceVal   = 100;
		listLength = 0;
	}

	SourceIniData& operator =(const SourceIniData& obj)
	{
		if (this != &obj)
		{
			this->sourceName = obj.sourceName;
			this->cycleType = obj.cycleType;
			this->voiceVal = obj.voiceVal;
			this->listLength = obj.listLength;
			this->mediaSourceList = obj.mediaSourceList;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(SourceIniData)

struct TextSourceIniData
{
	QString sourceName;
	QString text;
	QString path;

	TextSourceIniData& operator =(const TextSourceIniData& obj)
	{
		if (this != &obj)
		{
			this->sourceName = obj.sourceName;
			this->text = obj.text;
			this->path = obj.path;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(TextSourceIniData)

struct CameraCacheData
{
	QString sourceName;
	int cameraItemIdx;
	int resoultionItemIdx;
	bool filterOfBgSelected;
	int filterOfBgColor;
	int similarityVal;
	int smoothnessVal;
	int spillVal;
	int flipTypeVal;

	CameraCacheData() {
		cameraItemIdx = 0;
		resoultionItemIdx = 0;
		filterOfBgSelected = false;
		filterOfBgColor = color_to_int(Qt::green);
		similarityVal = 1;
		smoothnessVal = 1;
		spillVal = 1;
		flipTypeVal = 0;
	}

	CameraCacheData& operator =(const CameraCacheData& obj)
	{
		if (this != &obj)
		{
			this->sourceName = obj.sourceName;
			this->cameraItemIdx = obj.cameraItemIdx;
			this->resoultionItemIdx = obj.resoultionItemIdx;
			this->filterOfBgSelected = obj.filterOfBgSelected;
			this->filterOfBgColor = obj.filterOfBgColor;
			this->similarityVal = obj.similarityVal;
			this->smoothnessVal = obj.smoothnessVal;
			this->spillVal = obj.spillVal;
			this->flipTypeVal = obj.flipTypeVal;
		}
		return *this;
	}
};
Q_DECLARE_METATYPE(CameraCacheData)