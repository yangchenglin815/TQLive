/***********************************************************************************
*								UserConfig.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 用户配置文件管理器实现
***********************************************************************************/
#include "UserConfig.h"
#include "DataPathManager.h"
#include "EnumData.h"

#include <Windows.h>
#include <QTextCodec>
#include <QCoreApplication>
#include <QFileInfo>
#include <QPixmap>

UserConfig* UserConfig::m_pInstance = NULL;

UserConfig* UserConfig::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new UserConfig;
	}
	return m_pInstance;
}

void UserConfig::load()
{
	loadSectionbasic();
	loadSectionVideoOutput();
	loadMaterialIni();
	loadSoundEffectIni();
	loadLoopIni();
	loadHotKeyIni();
}

QVector<MaterialTypeData> UserConfig::getMaterialIni()
{
	return m_pMaterielTypeData;
}

QVector<SoundEffectTypeData> UserConfig::getSoundEffectIni()
{
	return m_pSoundEffcetTypeData;
}

QVector<LoopTypeData> UserConfig::getLoopIni()
{
	return m_pLoopTypeData;
}

int UserConfig::getGroupId()
{
	return m_groupId;
}

void UserConfig::setGroupId(int groupId)
{
	m_ConfigIni->setValue("basic/groupId", groupId);
	m_groupId = groupId;
}

QString UserConfig::getGroupNumber()
{
	return m_groupNumber;
}

void UserConfig::setGroupNumber(QString groupNumber)
{
	m_ConfigIni->setValue("basic/groupNumber", groupNumber);
	m_groupNumber = groupNumber;
}

bool UserConfig::getExitTipsFlag()
{
	return m_exitTipsFlag;
}

void UserConfig::setExitTipsFlag(bool flag)
{
	m_ConfigIni->setValue("basic/isShowExitTips", flag);
	m_exitTipsFlag = flag;
}

int UserConfig::getExitTipsType()
{
	return m_exitTipsType;
}

void UserConfig::setExitTipsType(int type)
{
	m_ConfigIni->setValue("basic/exitTipsType", type);
	m_exitTipsType = type;
}

int UserConfig::getPicQualityIndex()
{
	return m_picQualityIndex;
}

void UserConfig::setPicQualityIndex(int index)
{
	m_ConfigIni->setValue("video/picQualityIndex", index);
	m_picQualityIndex = index;
}

int UserConfig::getCurMICValue()
{
	return m_curMICValue;
}

void UserConfig::setCurMICValue(int val)
{
	m_ConfigIni->setValue("video/micValue", val);
	m_curMICValue = val;
}

int UserConfig::getCurJBLValue()
{
	return m_curJBLValue;
}

void UserConfig::setCurJBLValue(int val)
{
	m_ConfigIni->setValue("video/jblValue", val);
	m_curJBLValue = val;
}

bool UserConfig::getIsAutoSetSound()
{
	return m_bIsAutoSetSound;
}

void UserConfig::setIsAutoSetSound(bool flag)
{
	m_ConfigIni->setValue("video/isAutoSetSound", flag);
	m_bIsAutoSetSound = flag;
}

bool UserConfig::getIsOpenHotKey()
{
	return m_bIsOpenHotKey;
}

void UserConfig::setIsOpenHotKey(bool open)
{
	m_ConfigIni->setValue("HotKeyIni/isOpenHotKey", open);
	m_bIsOpenHotKey = open;
}

QString UserConfig::getMicHotKey()
{
	return m_sMicHotKey;
}

void UserConfig::setMicHotKey(QString hotKeyStr)
{
	m_ConfigIni->setValue("HotKeyIni/micHotKey", hotKeyStr);
	m_sMicHotKey = hotKeyStr;
}

QString UserConfig::getLiveHotKey()
{
	return m_sLiveHotKey;
}

void UserConfig::setLiveHotKey(QString hotKeyStr)
{
	m_ConfigIni->setValue("HotKeyIni/liveHotKey", hotKeyStr);
	m_sLiveHotKey = hotKeyStr;
}

QString UserConfig::getBarrageHotKey()
{
	return m_sBarrageHotKey;
}

void UserConfig::setBarrageHotKey(QString hotKeyStr)
{
	m_ConfigIni->setValue("HotKeyIni/barrageHotKey", hotKeyStr);
	m_sBarrageHotKey = hotKeyStr;
}

UserConfig::UserConfig(QObject *parent)
	: QObject(parent)
{
	m_configPath = QString::fromStdString(DataPathManager::getUserDir()) + "\\config.ini";
	m_ConfigIni  = new QSettings(m_configPath, QSettings::IniFormat);
	m_ConfigIni->setIniCodec(QTextCodec::codecForName("GB2312"));

	QFileInfo fileInfo(m_configPath);
	if (!fileInfo.isFile())
	{
		setGroupId(-1);
		setGroupNumber("");
		setExitTipsFlag(true);
		setExitTipsType(EXIT_TO_TRAY);
		setPicQualityIndex(PICQUALITY_CUSTOMIZE);
		setCurMICValue(100);
		setCurJBLValue(100);
		setIsAutoSetSound(false);
		setIsOpenHotKey(false);
		setMicHotKey("");
		setLiveHotKey("");
		setBarrageHotKey("");
	}
}

UserConfig::~UserConfig()
{

}

void UserConfig::loadMaterialIni()
{
	MaterialTypeData cameraData;
	cameraData.nType      = MATERIAL_CAMERA;
	cameraData.sName      = QStringLiteral("摄像头");
	cameraData.sIcon      = QString(":/TQLive/images/live tool_icon_camera_normal.png");
	cameraData.sHoverIcon = QString(":/TQLive/images/live tool_icon_camera_hover.png");
	m_pMaterielTypeData.push_back(cameraData);

	MaterialTypeData videoData;
	videoData.nType      = MATERIAL_VIDEO;
	videoData.sName      = QStringLiteral("视频");
	videoData.sIcon      = QString(":/TQLive/images/live tool_icon_video_normal.png");
	videoData.sHoverIcon = QString(":/TQLive/images/live tool_icon_video_hover.png");
	m_pMaterielTypeData.push_back(videoData);

	MaterialTypeData imageData;
	imageData.nType      = MATERIAL_IMAGE;
	imageData.sName      = QStringLiteral("图片");
	imageData.sIcon      = QString(":/TQLive/images/live tlool_icon_picture_normal.png");
	imageData.sHoverIcon = QString(":/TQLive/images/live tlool_icon_picture_hover.png");
	m_pMaterielTypeData.push_back(imageData);

	MaterialTypeData textData;
	textData.nType      = MATERIAL_TEXT;
	textData.sName      = QStringLiteral("文本");
	textData.sIcon      = QString(":/TQLive/images/live_tool_icon_text_normal.png");
	textData.sHoverIcon = QString(":/TQLive/images/live_tool_icon_text_hover.png");
	m_pMaterielTypeData.push_back(textData);

	MaterialTypeData sourceData;
	sourceData.nType      = MATERIAL_SOURCE;
	sourceData.sName      = QStringLiteral("直播源");
	sourceData.sIcon      = QString(":/TQLive/images/live tool_icon_source_normal.png");
	sourceData.sHoverIcon = QString(":/TQLive/images/live tool_icon_source_hover.png");
	m_pMaterielTypeData.push_back(sourceData);

	MaterialTypeData captureData;
	captureData.nType      = MATERIAL_CAPTURE;
	captureData.sName      = QStringLiteral("截屏");
	captureData.sIcon      = QString(":/TQLive/images/live tool_icon_screenshots_normal.png");
	captureData.sHoverIcon = QString(":/TQLive/images/live tool_icon_screenshots_hover.png");
	m_pMaterielTypeData.push_back(captureData);

	MaterialTypeData fullScreenData;
	fullScreenData.nType      = MATERIAL_FULLSCREEN;
	fullScreenData.sName      = QStringLiteral("全屏");
	fullScreenData.sIcon      = QString(":/TQLive/images/live tool_icon_full_normal.png");
	fullScreenData.sHoverIcon = QString(":/TQLive/images/live tool_icon_full_hover.png");
	m_pMaterielTypeData.push_back(fullScreenData);

	MaterialTypeData windowData;
	windowData.nType      = MATERIAL_WINDOW;
	windowData.sName      = QStringLiteral("窗口");
	windowData.sIcon      = QString(":/TQLive/images/window_icon_full_normal.png");
	windowData.sHoverIcon = QString(":/TQLive/images/window_icon_full_hover.png");
	m_pMaterielTypeData.push_back(windowData);
}

void UserConfig::loadSoundEffectIni()
{
	SoundEffectTypeData cheerData;
	cheerData.qSoundEffectName = QStringLiteral("欢呼");
	cheerData.qHotKey          = QStringLiteral("ctrl+1");
	cheerData.qAduioFile       = QStringLiteral("./Resource/欢呼.wav");
	m_pSoundEffcetTypeData.push_back(cheerData);

	SoundEffectTypeData goForItData;
	goForItData.qSoundEffectName = QStringLiteral("加油");
	goForItData.qHotKey          = QStringLiteral("ctrl+2");
	goForItData.qAduioFile       = QStringLiteral("./Resource/加油.wav");
	m_pSoundEffcetTypeData.push_back(goForItData);

	SoundEffectTypeData hissData;
	hissData.qSoundEffectName = QStringLiteral("嘘声");
	hissData.qHotKey          = QStringLiteral("ctrl+3");
	hissData.qAduioFile       = QStringLiteral("./Resource/嘘声.wav");
	m_pSoundEffcetTypeData.push_back(hissData);
}

void UserConfig::loadLoopIni()
{
	LoopTypeData randomData;
	randomData.nType = LOOP_RANDOM;
	randomData.sName = QStringLiteral("随机播放");
	randomData.sIcon = QString(":/TQLive/images/video_icon_random_normal.png");
	randomData.sHoverIcon = QString(":/TQLive/images/video_icon_random_hover.png");
	m_pLoopTypeData.push_back(randomData);
	
	LoopTypeData singleData;
	singleData.nType = LOOP_SINGLE;
	singleData.sName = QStringLiteral("单个循环");
	singleData.sIcon = QString(":/TQLive/images/video_icon_a single_normal.png");
	singleData.sHoverIcon = QString(":/TQLive/images/video_icon_a single_hover.png");
	m_pLoopTypeData.push_back(singleData);

	LoopTypeData cycleData;
	cycleData.nType = LOOP_CYCLE;
	cycleData.sName = QStringLiteral("列表循环");
	cycleData.sIcon = QString(":/TQLive/images/video_icon_cycle_normal.png");
	cycleData.sHoverIcon = QString(":/TQLive/images/video_icon_cycle_hover.png");
	m_pLoopTypeData.push_back(cycleData);
}

void UserConfig::loadSectionbasic()
{
	m_groupId      = m_ConfigIni->value("basic/groupId").toInt();
	m_exitTipsFlag = m_ConfigIni->value("basic/isShowExitTips").toBool();
	m_exitTipsType = m_ConfigIni->value("basic/exitTipsType").toInt();
	m_groupNumber  = m_ConfigIni->value("basic/groupNumber").toString();
}

void UserConfig::loadSectionVideoOutput()
{
	m_picQualityIndex = m_ConfigIni->value("video/picQualityIndex").toInt();
	m_curMICValue = m_ConfigIni->value("video/micValue").toInt();
	m_curJBLValue = m_ConfigIni->value("video/jblValue").toInt();
	m_bIsAutoSetSound = m_ConfigIni->value("video/isAutoSetSound").toBool();
}

void UserConfig::loadHotKeyIni()
{
	m_bIsOpenHotKey = m_ConfigIni->value("HotKeyIni/isOpenHotKey").toBool();
	m_sMicHotKey = m_ConfigIni->value("HotKeyIni/micHotKey").toString();
	m_sLiveHotKey = m_ConfigIni->value("HotKeyIni/liveHotKey").toString();
	m_sBarrageHotKey = m_ConfigIni->value("HotKeyIni/barrageHotKey").toString();
}
