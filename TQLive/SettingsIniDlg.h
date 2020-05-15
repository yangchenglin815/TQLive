#pragma once

#include <QDialog>
#include <QMovie>
#include <obs.hpp>
#include "ui_SettingsIniDlg.h"
#include "window-basic-main.hpp"
#include "window-basic-settings.hpp"

#define QT_UTF8(str) QString::fromUtf8(str)
#define QT_TO_UTF8(str) str.toUtf8().constData()

class SettingsIniDlg : public QDialog
{
	Q_OBJECT

public:
	SettingsIniDlg(QWidget *parent);
	~SettingsIniDlg();

	void InitPage();

signals:
	void onLiveRoomTitleChanged();
	void onHotKeyChanged(QString micHotKey, QString liveHotKey, QString barrageHotKey);
	void onShowMouseStateChanged(int state);
	void onWaterMarkStateChanged(int state);

private slots:
    //home
    void onConfirmClicked();
	void onClearStatistics();
	void onInitStatistics();

	//page-live-picQuality
	void onInitLiveParams(int index);

	//page-live-room
	void onEditRoomTitleClicked();
	void onSaveRoomTitleClicked();
	void onCancelRoomTitleClicked();
	void onEditLivePrevueClicked();
	void onSaveLivePrevueClicked();
	void onCancelLivePrevueClicked();
	void onUpdateLiveCoverImg();
	void onSetRoomCoverClicked();
	void onSetLiveTrailClicked();
	void onSetDefaultCover(bool checked);
	void onSetCustomCover(bool checked);

    //page-live-dir
    void onChangeDirOfLive();
    void onOpenDirOfLive();

	//page-sound
	void onAutoSetSoundChanged(int state);
	
	//page-hotKey
	void onHotKeySwitchChanged(int state);

	//network
	void onSlotGetOSSInfo(int nStatusCode, const QString &sMsg, const ST_OSS_INFORMATION &ossInfo, const void *userData);
	void onSlotUploadFileToOSS(int nStatusCode, const QString &sMsg, const QString &fileID, const void *userData);

	void onSlotWaterMarkStateChanged(int state);

private:
	void InitUI();
	void InitConnect();

	void loadLiveRoomSettings();
	void loadLivePicQualitySettings();
	void loadLiveContentSettings();
	void loadLiveVoiceSettings();
	void loadLiveHotKeySettings();
	void loadVideoDirSettings();
	void loadOtherSettings();

	void saveLiveRoomSettings();
	void saveLivePicQualitySettings();
	void saveLiveContentSettings();
	void saveLiveVoiceSettings();
	void saveLiveHotKeySettings();
	void saveVideoDirSettings();
	void saveOtherSettings();

	void setCustomizeParams(bool isCustomized);
	void AddComboItem(QComboBox *combo, const char *text, const char *val);
	void AddComboItems(QComboBox *combo, obs_property_t *prop, int index);
	void RestrictResetBitrates(QComboBox *combo, int maxbitrate);
	void AddFilterForAudioSource(const char *id, obs_source_t *source);
	void InitNoiseSuppress(obs_source_t *source);

private:
	Ui::SettingsIniDlg ui;
	OBSBasic *main;
	QMovie *m_uploadMovie;
	obs_data_t *m_pAudioSettings;
	QString m_sCoverUrl;
	QString m_sCoverLocalUrl;

	bool m_pIsLiving = false;
	bool m_liveRoom_isModify;
	bool m_livePrevue_isModify;
	bool m_liveCover_isModify;
};
