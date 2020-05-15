#pragma once

#include <QObject>
#include <QSettings>
#include <QVector>
#include "StructData.h"

class UserConfig : public QObject
{
	Q_OBJECT

public:
	static UserConfig* instance();
	void load();
	QVector<MaterialTypeData> getMaterialIni();
	QVector<SoundEffectTypeData> getSoundEffectIni();
	QVector<LoopTypeData> getLoopIni();

	//////////////////////// config配置文件 ////////////////////////
	//general 
	int getGroupId();
	void setGroupId(int groupId);

	QString getGroupNumber();
	void setGroupNumber(QString groupNumber);

	bool getExitTipsFlag();
	void setExitTipsFlag(bool flag);

	int getExitTipsType();
	void setExitTipsType(int type);

	//ouput
	int getPicQualityIndex();
	void setPicQualityIndex(int index);

	int getCurMICValue();
	void setCurMICValue(int val);

	int getCurJBLValue();
	void setCurJBLValue(int val);

	bool getIsAutoSetSound();
	void setIsAutoSetSound(bool flag);

	//HotKey
	bool getIsOpenHotKey();
	void setIsOpenHotKey(bool open);

	QString getMicHotKey();
	void setMicHotKey(QString hotKeyStr);

	QString getLiveHotKey();
	void setLiveHotKey(QString hotKeyStr);

	QString getBarrageHotKey();
	void setBarrageHotKey(QString hotKeyStr);

private:
	explicit UserConfig(QObject *parent = 0);
	virtual ~UserConfig();

	/*加载素材配置*/
	void loadMaterialIni();
	/*加载音效配置*/
	void loadSoundEffectIni();
	/*加载列表循环配置*/
	void loadLoopIni();
	/*加载通用的字段信息*/
	void loadSectionbasic();
	/*加载视频输出配置*/
	void loadSectionVideoOutput();
	/*加载快捷键配置*/
	void loadHotKeyIni();

private:
	static UserConfig* m_pInstance;
	QString m_configPath;
	QSettings* m_ConfigIni;
	QVector<MaterialTypeData> m_pMaterielTypeData;
	QVector<SoundEffectTypeData> m_pSoundEffcetTypeData;
	QVector<LoopTypeData> m_pLoopTypeData;

	//////////////////////// config配置文件 ////////////////////////
	//general
	int m_groupId;
	bool m_exitTipsFlag;
	int m_exitTipsType;
	int m_picQualityIndex;
	QString m_groupNumber;
	
	//output
	int m_curMICValue;
	int m_curJBLValue;
	bool m_bIsAutoSetSound;

	//HotKey
	bool m_bIsOpenHotKey;
	QString m_sMicHotKey;
	QString m_sLiveHotKey;
	QString m_sBarrageHotKey;
};
