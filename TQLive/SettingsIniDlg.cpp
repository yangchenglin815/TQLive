/***********************************************************************************
*								SettingsIniDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 系统设置对话框
***********************************************************************************/
#include "SettingsIniDlg.h"
#include "FramelessHelper.h"
#include "UserConfig.h"
#include "EnumData.h"
#include "window-basic-settings.hpp"
#include "CDataCentre.h"
#include "HttpAgent.h"
#include "ConstData.h"
#include "CommonFunc.h"
#include "UserConfig.h"
#include "GlobalSignal.h"
#include "audio-encoders.hpp"

#include <QFileDialog>
#include <QDesktopServices>
#include <QToolTip>
#include <QStandardItemModel>
#include <QKeyEvent>

#include <QJsonObject>
#include <QJsonDocument>
#include <QImage>
#include <QImageReader>
#include <QPainter>

static inline bool IsSurround(const char *speakers)
{
	static const char *surroundLayouts[] = {
		"2.1",
		"4.0",
		"4.1",
		"5.1",
		"7.1",
		nullptr
	};

	if (!speakers || !*speakers)
		return false;

	const char **curLayout = surroundLayouts;
	for (; *curLayout; ++curLayout) {
		if (strcmp(*curLayout, speakers) == 0) {
			return true;
		}
	}

	return false;
}

static inline void SetComboByName(QComboBox *combo, const char *name)
{
	int idx = combo->findText(QT_UTF8(name));
	if (idx != -1)
		combo->setCurrentIndex(idx);
}

static void PopulateAACBitrates(QComboBox *combo)
{
	auto &bitrateMap = GetAACEncoderBitrateMap();
	if (bitrateMap.empty())
		return;

	vector<pair<QString, QString>> pairs;
	for (auto &entry : bitrateMap)
		pairs.emplace_back(QString::number(entry.first),
			obs_encoder_get_display_name(entry.second));

	QString currentText = combo->currentText();
	combo->clear();

	for (auto &pair : pairs) {
		combo->addItem(pair.first);
		combo->setItemData(combo->count() - 1, pair.second,
			Qt::ToolTipRole);
	}

	combo->setCurrentText(currentText);
}

static inline QString GetComboData(QComboBox *combo)
{
	int idx = combo->currentIndex();
	if (idx == -1)
		return QString();

	return combo->itemData(idx).toString();
}

SettingsIniDlg::SettingsIniDlg(QWidget *parent)
	: QDialog(parent)
	, main(qobject_cast<OBSBasic*>(parent))
	, m_liveRoom_isModify(false)
	, m_livePrevue_isModify(false)
	, m_liveCover_isModify(false)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	InitUI();
	InitConnect();
}

SettingsIniDlg::~SettingsIniDlg()
{

}

void SettingsIniDlg::InitPage()
{
	loadLiveRoomSettings();
	loadLivePicQualitySettings();
	loadLiveContentSettings();
	loadLiveVoiceSettings();
	loadLiveHotKeySettings();
	loadVideoDirSettings();
	loadOtherSettings();

	this->exec();
}

void SettingsIniDlg::onConfirmClicked()
{
	this->close();

	saveLiveRoomSettings();
	saveLivePicQualitySettings();
	saveVideoDirSettings();
	saveLiveContentSettings();
	saveLiveVoiceSettings();
	saveLiveHotKeySettings();
	saveOtherSettings();	

	main->ResetVideo();
	config_save_safe(main->Config(), "tmp", nullptr);
	config_save_safe(GetGlobalConfig(), "tmp", nullptr);
	main->SaveProject();
}

void SettingsIniDlg::onClearStatistics()
{
	m_pIsLiving = false;
}

void SettingsIniDlg::onInitStatistics()
{
	m_pIsLiving = true;
}

void SettingsIniDlg::onInitLiveParams(int index)
{
	switch (index)
	{
	case PICQUALITY_BD_4M:
		ui.comboBox_bitrate_val->setCurrentIndex(BITRATE_4000);
		ui.comboBox_frame_val->setCurrentIndex(FRAME_INTERVAL_30);
		ui.comboBox_resolution_val->setCurrentIndex(RESOLUTION_1080P);
		setCustomizeParams(false);
		break;
	case PICQUALITY_BD:
		ui.comboBox_bitrate_val->setCurrentIndex(BITRATE_3000);
		ui.comboBox_frame_val->setCurrentIndex(FRAME_INTERVAL_30);
		ui.comboBox_resolution_val->setCurrentIndex(RESOLUTION_1080P);
		setCustomizeParams(false);
		break;
	case PICQUALITY_FHD:
		ui.comboBox_bitrate_val->setCurrentIndex(BITRATE_2000);
		ui.comboBox_frame_val->setCurrentIndex(FRAME_INTERVAL_30);
		ui.comboBox_resolution_val->setCurrentIndex(RESOLUTION_1080P);
		setCustomizeParams(false);
		break;
	case PICQUALITY_HD:
		ui.comboBox_bitrate_val->setCurrentIndex(BITRATE_1500);
		ui.comboBox_frame_val->setCurrentIndex(FRAME_INTERVAL_30);
		ui.comboBox_resolution_val->setCurrentIndex(RESOLUTION_720P);
		setCustomizeParams(false);
		break;
	case PICQUALITY_SD:
		ui.comboBox_bitrate_val->setCurrentIndex(BITRATE_800);
		ui.comboBox_frame_val->setCurrentIndex(FRAME_INTERVAL_25);
		ui.comboBox_resolution_val->setCurrentIndex(RESOLUTION_720P);
		setCustomizeParams(false);
		break;
	case PICQUALITY_CUSTOMIZE:
		ui.comboBox_bitrate_val->setCurrentIndex(BITRATE_3000);
		ui.comboBox_frame_val->setCurrentIndex(FRAME_INTERVAL_30);
		ui.comboBox_resolution_val->setCurrentIndex(RESOLUTION_1080P);
		setCustomizeParams(true);
		break;
	default:
		break;
	}
}

void SettingsIniDlg::onEditRoomTitleClicked()
{
	ui.lineEdit_roomTitle->clear();
	ui.stackedWidget->setCurrentWidget(ui.page_edit_roomTitle);
}

void SettingsIniDlg::onSaveRoomTitleClicked()
{
	QString liveRoomName = ui.lineEdit_roomTitle->text();
	int length = ui.lineEdit_roomTitle->text().length();
	if (length < 5)
	{
		ui.lineEdit_roomTitle->clear();
		ui.lineEdit_roomTitle->setFocus();

		QFont font;
		font.setFamily(QString("Microsoft YaHei"));
		font.setPixelSize(14);
		QToolTip::setFont(font);
		QToolTip::showText(ui.lineEdit_roomTitle->mapToGlobal(ui.lineEdit_roomTitle->pos()), QStringLiteral("请输入5~20字的房间名称！"));
	}
	else
	{
		ui.label_roomTitle_val->setText(liveRoomName);
		ui.stackedWidget->setCurrentWidget(ui.page_roomTitle);

		m_liveRoom_isModify = true;
	}
}

void SettingsIniDlg::onCancelRoomTitleClicked()
{
	ui.stackedWidget->setCurrentWidget(ui.page_roomTitle);
}

void SettingsIniDlg::onEditLivePrevueClicked()
{
	ui.lineEdit_livePrevue->clear();
	ui.stackedWidget_2->setCurrentWidget(ui.page_edit_livePrevue);
}

void SettingsIniDlg::onSaveLivePrevueClicked()
{
	QString livePrevue = ui.lineEdit_livePrevue->text();
	if (livePrevue.isEmpty())
	{
		ui.lineEdit_livePrevue->setFocus();

		QFont font;
		font.setFamily(QString("Microsoft YaHei"));
		font.setPixelSize(14);
		QToolTip::setFont(font);
		QToolTip::showText(ui.lineEdit_livePrevue->mapToGlobal(ui.lineEdit_livePrevue->pos()), QStringLiteral("请输入直播预告！"));
	}
	else
	{
		ui.label_livePrevue_val->setText(livePrevue);
		ui.stackedWidget_2->setCurrentWidget(ui.page_livePrevue);

		m_livePrevue_isModify = true;
		main->m_bLivetrailerClear = false;
	}
}

void SettingsIniDlg::onCancelLivePrevueClicked()
{
	ui.stackedWidget_2->setCurrentWidget(ui.page_livePrevue);
}

void SettingsIniDlg::onUpdateLiveCoverImg()
{
	m_sCoverLocalUrl = QFileDialog::getOpenFileName(NULL,
		QT_UTF8(""), QT_UTF8(""),
		QT_UTF8(""));


    if (m_sCoverLocalUrl.isEmpty())
       return;

	ui.widget_roomCover->setCurrentWidget(ui.page_roomCoverInit);
	m_uploadMovie->start();

	HttpAgent::instance()->RequestOnGetOSSInfo();
}

void SettingsIniDlg::onSetRoomCoverClicked()
{
	QString url = "";

	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		url = LIVE_SETTINGS_TEST_API;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		url = LIVE_SETTINGS_API;
	}

	if (!url.isEmpty())
	{
		QDesktopServices::openUrl(QUrl(url));
	}
}

void SettingsIniDlg::onSetLiveTrailClicked()
{
	QString url = "";

	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		url = LIVE_SETTINGS_TEST_API;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		url = LIVE_SETTINGS_API;
	}

	if (!url.isEmpty())
	{
		QDesktopServices::openUrl(QUrl(url));
	}
}

void SettingsIniDlg::onSetDefaultCover(bool checked)
{
	if (checked) {
		ui.widget_roomCover->setHidden(true);
	}
}

void SettingsIniDlg::onSetCustomCover(bool checked)
{
	if (checked) {
		ui.widget_roomCover->setHidden(false);
	}
}

void SettingsIniDlg::onChangeDirOfLive()
{
	QString fileName = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择文件路径..."), "./");
	if (!fileName.isEmpty())
	{
		ui.lineEdit_dir->setText(fileName);
	}
}

void SettingsIniDlg::onOpenDirOfLive()
{
	if (!ui.lineEdit_dir->text().isEmpty())
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(ui.lineEdit_dir->text()));
	}
}

void SettingsIniDlg::onAutoSetSoundChanged(int state)
{
	if (state == Qt::Checked)
	{
		auto idx = ui.comboBox_mic->findText(QStringLiteral("默认"));
		ui.comboBox_mic->setCurrentIndex(idx);
		ui.comboBox_mic->setDisabled(true);

		idx = ui.comboBox_jbl->findText(QStringLiteral("默认"));
		ui.comboBox_jbl->setCurrentIndex(idx);
		ui.comboBox_jbl->setDisabled(true);
	}
	else
	{
		ui.comboBox_mic->setDisabled(false);
		ui.comboBox_jbl->setDisabled(false);
	}
}

void SettingsIniDlg::onHotKeySwitchChanged(int state)
{
	bool checked = false;
	QString color = QString("#777777");
	if (state == Qt::Checked) {
		checked = true;
		color = QString("#999999");
	}

	QString style = QString("QLabel{ font-family:Microsoft YaHei; font-size:12px; \
 color:%1; background:transparent; }").arg(color);
	ui.label_mic_switch->setStyleSheet(style);
	ui.label_live_switch->setStyleSheet(style);
	ui.label_barrage_switch->setStyleSheet(style);

	ui.lineEdit_mic_hotKey->setDisabled(!checked);
	ui.lineEdit_live_hotKey->setDisabled(!checked);
	ui.lineEdit_barrage_hotKey->setDisabled(!checked);
}

void SettingsIniDlg::onSlotGetOSSInfo(int nStatusCode, const QString &sMsg, const ST_OSS_INFORMATION &ossInfo, const void *userData)
{
	if (nStatusCode == 200)
	{
		HttpAgent::instance()->RequestOnUploadFileToOSS(ossInfo, m_sCoverLocalUrl);
	}
	else
	{
		m_uploadMovie->stop();
		GlobalSignal::instance()->onSigCatchException(nStatusCode, sMsg);
	}
}

void SettingsIniDlg::onSlotUploadFileToOSS(int nStatusCode, const QString &sMsg, const QString &fileID, const void *userData)
{
	m_uploadMovie->stop();

	if (nStatusCode == 200)
	{
		m_sCoverUrl = fileID;
		m_liveCover_isModify = true;
		g_dataCentre.stUserInformation.qsCoverURL = m_sCoverUrl;

		ui.page_roomCoverLoad->setCoverImg(m_sCoverLocalUrl);
		ui.widget_roomCover->setCurrentWidget(ui.page_roomCoverLoad);
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatusCode, sMsg);
	}
}

void SettingsIniDlg::onSlotWaterMarkStateChanged(int state)
{
	return;
	int value = 0;
	if (state == Qt::Checked)
		value = 1;

	g_dataCentre.stUserInformation.nShowRoomNumber = value;
	HttpAgent::instance()->RequestOnWaterMarkFlags(QString::number(value));

	emit onWaterMarkStateChanged(value);
}

void SettingsIniDlg::InitUI()
{
	ui.btn_live_room->setChecked(true);
	ui.wgt_right->setCurrentWidget(ui.page_live_room);
	ui.wgt_cycle_time->setHidden(true);

	//page-live-room
	ui.lineEdit_roomTitle->setMaxLength(20);	

	//page-live-picQuality
	ui.comboBox_quality_val->setView(new QListView());
	ui.comboBox_bitrate_val->setView(new QListView());
	ui.comboBox_frame_val->setView(new QListView());
	ui.comboBox_resolution_val->setView(new QListView());
	ui.comboBox_encoder_val->setView(new QListView());

	//page-live-dir
	ui.checkBox_cycle_time->setChecked(false);
	ui.spinBox_cycle_time->setValue(60);

	//page-live-other
	ui.radioButton_min->setChecked(true);

	ui.btn_close->setCursor(Qt::PointingHandCursor);
	ui.btn_cancel->setCursor(Qt::PointingHandCursor);
	ui.btn_confirm->setCursor(Qt::PointingHandCursor);
	ui.btn_change_dir->setCursor(Qt::PointingHandCursor);
	ui.btn_live_content->setCursor(Qt::PointingHandCursor);
	ui.btn_live_dir->setCursor(Qt::PointingHandCursor);
	ui.btn_live_hotKey->setCursor(Qt::PointingHandCursor);
	ui.btn_live_other->setCursor(Qt::PointingHandCursor);
	ui.btn_live_quality->setCursor(Qt::PointingHandCursor);
	ui.btn_live_room->setCursor(Qt::PointingHandCursor);
	ui.btn_live_voice->setCursor(Qt::PointingHandCursor);
	ui.btn_open_dir->setCursor(Qt::PointingHandCursor);
	ui.button_cancel_livePrevue->setCursor(Qt::PointingHandCursor);
	ui.button_cancel_roomTitle->setCursor(Qt::PointingHandCursor);
	ui.button_edit_livePrevue->setCursor(Qt::PointingHandCursor);
	ui.button_edit_roomTitle->setCursor(Qt::PointingHandCursor);
	ui.button_save_livePrevue->setCursor(Qt::PointingHandCursor);
	ui.button_save_roomTitle->setCursor(Qt::PointingHandCursor);
	ui.button_updateImg->setCursor(Qt::PointingHandCursor);

	m_uploadMovie = new QMovie(":/TQLive/images/uploadProgress.gif");
	ui.label_upload_progress->setMovie(m_uploadMovie);
	ui.widget_roomCover->setCurrentWidget(ui.page_roomCoverInit);
	//m_uploadMovie->start();

	/*隐藏部分功能，二期开发*/
	ui.checkBox_show_mouse->hide();

	ui.radioButton_default->setChecked(true);
	onSetDefaultCover(true);
}

void SettingsIniDlg::InitConnect()
{
	//page-home
	connect(ui.btn_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.btn_cancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
	connect(GlobalSignal::instance(), SIGNAL(onSigClearStatisticsData()), this, SLOT(onClearStatistics()));
	connect(GlobalSignal::instance(), SIGNAL(onSigInitStatisticsData()), this, SLOT(onInitStatistics()));

	//page-live-room
	connect(ui.btn_live_room, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_room);
	});
	connect(ui.button_edit_roomTitle, SIGNAL(clicked()), this, SLOT(onEditRoomTitleClicked()));
	connect(ui.button_save_roomTitle, SIGNAL(clicked()), this, SLOT(onSaveRoomTitleClicked()));
	connect(ui.button_cancel_roomTitle, SIGNAL(clicked()), this, SLOT(onCancelRoomTitleClicked()));
	connect(ui.button_edit_livePrevue, SIGNAL(clicked()), this, SLOT(onEditLivePrevueClicked()));
	connect(ui.button_save_livePrevue, SIGNAL(clicked()), this, SLOT(onSaveLivePrevueClicked()));
	connect(ui.button_cancel_livePrevue, SIGNAL(clicked()), this, SLOT(onCancelLivePrevueClicked()));
	connect(ui.button_updateImg, SIGNAL(clicked()), this, SLOT(onUpdateLiveCoverImg()));
	connect(ui.button_selectImg, SIGNAL(clicked()), this, SLOT(onUpdateLiveCoverImg()));
	connect(ui.radioButton_default, SIGNAL(toggled(bool)), this, SLOT(onSetDefaultCover(bool)));
	connect(ui.radioButton_custom, SIGNAL(toggled(bool)), this, SLOT(onSetCustomCover(bool)));
	//connect(ui.button_edit_roomCover, SIGNAL(clicked()), this, SLOT(onSetRoomCoverClicked()));
	//connect(ui.button_edit_liveTrail, SIGNAL(clicked()), this, SLOT(onSetLiveTrailClicked()));

	//page-live-picQuality
	connect(ui.btn_live_quality, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_quality);
	});
	connect(ui.comboBox_quality_val, SIGNAL(currentIndexChanged(int)), this, SLOT(onInitLiveParams(int)));


	connect(ui.btn_live_content, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_content);
	});
	connect(ui.btn_live_voice, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_voice);
	});
	connect(ui.btn_live_hotKey, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_hotKey);
	});

	//page-live-dir
	connect(ui.btn_live_dir, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_dir);
	});
	connect(ui.btn_change_dir, SIGNAL(clicked()), this, SLOT(onChangeDirOfLive()));
	connect(ui.btn_open_dir, SIGNAL(clicked()), this, SLOT(onOpenDirOfLive()));

	//page-live-sound
	connect(ui.btn_live_voice, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_voice);
	});
	connect(ui.checkBox_autoSet, SIGNAL(stateChanged(int)), this, SLOT(onAutoSetSoundChanged(int)));

	//page-hotKey
	connect(ui.btn_live_hotKey, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_hotKey);
	});
	connect(ui.checkBox_hotKey, SIGNAL(stateChanged(int)), this, SLOT(onHotKeySwitchChanged(int)));

	//page-other
	connect(ui.btn_live_other, &QPushButton::clicked, this, [&]() {
		ui.wgt_right->setCurrentWidget(ui.page_live_other);
	});

	//page-content
	connect(ui.checkBox_show_mouse, SIGNAL(stateChanged(int)), this, SIGNAL(onShowMouseStateChanged(int)));
	connect(ui.checkBox_watermark, SIGNAL(stateChanged(int)), this, SLOT(onSlotWaterMarkStateChanged(int)));

	//network
	connect(HttpAgent::instance(), SIGNAL(onSigGetOSSInfo(int, const QString&, const ST_OSS_INFORMATION&, const void*)),
		this, SLOT(onSlotGetOSSInfo(int, const QString&, const ST_OSS_INFORMATION&, const void*)));
	connect(HttpAgent::instance(), SIGNAL(onSigUploadFileToOSS(int, const QString&, const QString&, const void*)),
		this, SLOT(onSlotUploadFileToOSS(int, const QString&, const QString&, const void*)));
}

void SettingsIniDlg::loadLiveRoomSettings()
{
	if (main->m_bLivetrailerClear)
		g_dataCentre.stUserInformation.qsLiveAdvanceNotice = "";

	m_liveRoom_isModify = false;
	m_livePrevue_isModify = false;
	m_liveCover_isModify = false;

	ui.label_roomNumber_val->setText(g_dataCentre.stUserInformation.qsRoomNumber);
	ui.label_roomTitle_val->setText(g_dataCentre.stUserInformation.qsRoomName);
	ui.label_livePrevue_val->setText(g_dataCentre.stUserInformation.qsLiveAdvanceNotice);

	EN_LIVE_ROOM_COVER_TYPE coverType = g_dataCentre.stUserInformation.eLiveRoomCoverType;
	QString coverUrl = g_dataCentre.stUserInformation.qsCoverURL;

	if (coverType == EN_LIVE_ROOM_COVER_TYPE::kLiveFrame)
	{
		ui.radioButton_default->setChecked(true);
		m_sCoverUrl = "";
	}
	else if (coverType == EN_LIVE_ROOM_COVER_TYPE::kCustomPicture)
	{
		ui.radioButton_custom->setChecked(true);
		ui.page_roomCoverLoad->setWebCoverImg(coverUrl);
		ui.widget_roomCover->setCurrentWidget(ui.page_roomCoverLoad);
		m_sCoverUrl = coverUrl;
	}
}

void SettingsIniDlg::loadLivePicQualitySettings()
{
	if (m_pIsLiving)
	{
		ui.comboBox_quality_val->setDisabled(true);
		ui.comboBox_frame_val->setDisabled(true);
		ui.comboBox_resolution_val->setDisabled(true);
		ui.comboBox_encoder_val->setDisabled(true);
	}
	else
	{
		ui.comboBox_quality_val->setDisabled(false);
		ui.comboBox_encoder_val->setDisabled(false);
		onInitLiveParams(ui.comboBox_quality_val->currentIndex());
	}

	ui.comboBox_quality_val->setCurrentIndex(UserConfig::instance()->getPicQualityIndex());

	int videoBitrate = config_get_uint(main->Config(), "SimpleOutput", "VBitrate");
	ui.comboBox_bitrate_val->setCurrentIndex(CommonFunc::getCurrentBitrateIndex(videoBitrate));

	const char *frameInterval = config_get_string(main->Config(), "Video", "FPSCommon");
	ui.comboBox_frame_val->setCurrentIndex(CommonFunc::getFrameIntervalIndex(QString(frameInterval)));

	uint32_t out_cx = config_get_uint(main->Config(), "Video", "OutputCX");
	uint32_t out_cy = config_get_uint(main->Config(), "Video", "OutputCY");
	string outputResString = CommonFunc::ResString(out_cx, out_cy);
	ui.comboBox_resolution_val->setCurrentIndex(CommonFunc::getResolutionIndex(outputResString));

	const char *streamEnc = config_get_string(main->Config(), "AdvOut", "Encoder");
	ui.comboBox_encoder_val->setCurrentIndex(CommonFunc::getAdOutputEncoderIndex(QString(streamEnc)));

	OBSData streamEncSettings = CommonFunc::GetDataFromJsonFile("streamEncoder.json");
	const char *rate_control = obs_data_get_string(streamEncSettings, "rate_control");
	int outputBitrateIndex = CommonFunc::getOutputBitrateIndex(QString(rate_control));
	switch (outputBitrateIndex)
	{
	case OUTPUT_BITRATE_VBR:
		ui.radioButton_vbr->setChecked(true);
		break;
	case OUTPUT_BITRATE_CBR:
		ui.radioButton_cbr->setChecked(true);
		break;
	default:
		break;
	}
}

void SettingsIniDlg::loadLiveContentSettings()
{
	ui.checkBox_watermark->setChecked(g_dataCentre.stUserInformation.nShowRoomNumber);

	//emit onWaterMarkStateChanged(g_dataCentre.stUserInformation.nShowRoomNumber);
}

void SettingsIniDlg::loadLiveVoiceSettings()
{
	const char *input_id  = App()->InputAudioSource();
	const char *output_id = App()->OutputAudioSource();

	obs_properties_t *input_props  = obs_get_source_properties(input_id);
	obs_properties_t *output_props = obs_get_source_properties(output_id);

	if (input_props)
	{
		obs_property_t *inputs = obs_properties_get(input_props,
			"device_id");
		AddComboItems(ui.comboBox_mic, inputs, 3);
		obs_properties_destroy(input_props);
	}

	if (output_props)
	{
		obs_property_t *outputs = obs_properties_get(output_props,
			"device_id");
		AddComboItems(ui.comboBox_jbl, outputs, 1);
		obs_properties_destroy(output_props);
	}

	PopulateAACBitrates(ui.comboBox_audio_codeRate);
	auto audioBitrate = config_get_uint(main->Config(), "SimpleOutput",
		"ABitrate");
	audioBitrate = FindClosestAvailableAACBitrate(audioBitrate);
	const char *speakers = config_get_string(main->Config(), "Audio",
		"ChannelSetup");

	// restrict list of bitrates when multichannel is OFF
	if (!IsSurround(speakers))
		RestrictResetBitrates(ui.comboBox_audio_codeRate, 320);

	SetComboByName(ui.comboBox_audio_codeRate,
		std::to_string(audioBitrate).c_str());

	ui.checkBox_autoSet->setChecked(UserConfig::instance()->getIsAutoSetSound());
}

void SettingsIniDlg::loadLiveHotKeySettings()
{
	ui.checkBox_hotKey->setChecked(UserConfig::instance()->getIsOpenHotKey());
	onHotKeySwitchChanged(ui.checkBox_hotKey->checkState());

	ui.lineEdit_mic_hotKey->setText(UserConfig::instance()->getMicHotKey());
	ui.lineEdit_live_hotKey->setText(UserConfig::instance()->getLiveHotKey());
	ui.lineEdit_barrage_hotKey->setText(UserConfig::instance()->getBarrageHotKey());
}

void SettingsIniDlg::loadVideoDirSettings()
{
	const char *path = config_get_string(main->Config(), "AdvOut", "RecFilePath");
	ui.lineEdit_dir->setText(path);
}

void SettingsIniDlg::loadOtherSettings()
{
	int exitType = UserConfig::instance()->getExitTipsType();
	if (exitType == EXIT_TO_TRAY)
	{
		ui.radioButton_min->setChecked(true);
	}
	else if (exitType == EXIT_APP)
	{
		ui.radioButton_exit->setChecked(true);
	}

	bool autoLogin = g_dataCentre.m_stCurrentAccountInfo.autoLoginMark;
	ui.checkBox_autoLogin->setChecked(autoLogin);
}

void SettingsIniDlg::saveLiveRoomSettings()
{
	if (m_liveRoom_isModify)
	{
		QString liveRoomName = ui.label_roomTitle_val->text();
		HttpAgent::instance()->RequestOnModifyLiveRoomParams(2, liveRoomName);
		g_dataCentre.stUserInformation.qsRoomName = liveRoomName;

		emit onLiveRoomTitleChanged();
	}

	if (m_livePrevue_isModify)
	{
		QString livePrevue = ui.label_livePrevue_val->text();
		HttpAgent::instance()->RequestOnModifyLiveRoomParams(1, livePrevue);
		g_dataCentre.stUserInformation.qsLiveAdvanceNotice = livePrevue;
	}

	if (ui.radioButton_default->isChecked())
	{
		g_dataCentre.stUserInformation.qsCoverURL = "";
		g_dataCentre.stUserInformation.eLiveRoomCoverType = EN_LIVE_ROOM_COVER_TYPE::kLiveFrame;

		HttpAgent::instance()->RequestOnModifyLiveRoomParams(4, "");
	}
	else if (ui.radioButton_custom->isChecked() && m_liveCover_isModify)
	{
		g_dataCentre.stUserInformation.qsCoverURL = m_sCoverUrl;
		g_dataCentre.stUserInformation.eLiveRoomCoverType = EN_LIVE_ROOM_COVER_TYPE::kCustomPicture;
		HttpAgent::instance()->RequestOnModifyLiveRoomParams(4, m_sCoverUrl);
	}
}

void SettingsIniDlg::saveLivePicQualitySettings()
{
	UserConfig::instance()->setPicQualityIndex(ui.comboBox_quality_val->currentIndex());

	int bitrate = CommonFunc::getCurrentBitrateByIndex(ui.comboBox_bitrate_val->currentIndex());
	config_set_int(main->Config(), "SimpleOutput", "VBitrate", bitrate);

	QString frameInterval = CommonFunc::getFrameIntervalByIndex(ui.comboBox_frame_val->currentIndex());
	config_set_string(main->Config(), "Video", "FPSCommon", QT_TO_UTF8(frameInterval));

	uint32_t cx, cy;
	QString outputResolution = CommonFunc::getResolutionByIndex(ui.comboBox_resolution_val->currentIndex());
	if (CommonFunc::ConvertResText(QT_TO_UTF8(outputResolution), cx, cy))
	{
		config_set_uint(main->Config(), "Video", "OutputCX", cx);
		config_set_uint(main->Config(), "Video", "OutputCY", cy);
	}

	QString outputEncoder = CommonFunc::getOutputEncoderByIndex(ui.comboBox_encoder_val->currentIndex());
	config_set_string(main->Config(), "SimpleOutput", "StreamEncoder", QT_TO_UTF8(outputEncoder));
	QString adOutputEncoder = CommonFunc::getAdOutputEocoderByIndex(ui.comboBox_encoder_val->currentIndex());
	config_set_string(main->Config(), "AdvOut", "Encoder", QT_TO_UTF8(adOutputEncoder));

	QString outputBitrateType = "";
	if (ui.radioButton_vbr->isChecked())
	{
		outputBitrateType = "VBR";
	}
	else if (ui.radioButton_cbr->isChecked())
	{
		outputBitrateType = "CBR";
	}
	if (!outputBitrateType.isEmpty())
	{
		QJsonObject json;
		json.insert("rate_control", outputBitrateType);
		json.insert("keyint_sec", 2);
		json.insert("bitrate", bitrate);

		if (ui.comboBox_encoder_val->currentIndex() == ENCODER_GPU && ui.radioButton_vbr->isChecked())
		{
			json.insert("max_bitrate", 5000);
		}

		QJsonDocument document;
		document.setObject(json);
		QByteArray byteArray = document.toJson(QJsonDocument::Compact);
		const char *data = byteArray.data();

		char full_path[512];
		bool flag = false;
		int ret = GetProfilePath(full_path, sizeof(full_path), "streamEncoder.json");
		if (ret > 0)
		{
			if (data && *data) {
				os_quick_write_utf8_file_safe(full_path, data, strlen(data), false, "tmp", "bak");
			}
		}
	}
}

void SettingsIniDlg::saveLiveContentSettings()
{
	int value = 0;
	if (ui.checkBox_watermark->checkState() == Qt::Checked)
		value = 1;

	g_dataCentre.stUserInformation.nShowRoomNumber = value;
	HttpAgent::instance()->RequestOnWaterMarkFlags(QString::number(value));

	emit onWaterMarkStateChanged(value);
}

void SettingsIniDlg::saveLiveVoiceSettings()
{
	UserConfig::instance()->setIsAutoSetSound(ui.checkBox_autoSet->isChecked());

	main->ResetAudioDevice(App()->InputAudioSource(), QT_TO_UTF8(GetComboData(ui.comboBox_mic)), Str("Basic.AuxDevice1"), 3);
	main->ResetAudioDevice(App()->OutputAudioSource(), QT_TO_UTF8(GetComboData(ui.comboBox_jbl)), Str("Basic.DesktopDevice1"), 1);
	main->SaveProject();

	config_set_string(main->Config(), "SimpleOutput", "ABitrate", QT_TO_UTF8(ui.comboBox_audio_codeRate->currentText()));
	main->ResetOutputs();
}

void SettingsIniDlg::saveLiveHotKeySettings()
{
	UserConfig::instance()->setIsOpenHotKey(ui.checkBox_hotKey->isChecked());
	UserConfig::instance()->setMicHotKey(ui.lineEdit_mic_hotKey->text());
	UserConfig::instance()->setLiveHotKey(ui.lineEdit_live_hotKey->text());
	UserConfig::instance()->setBarrageHotKey(ui.lineEdit_barrage_hotKey->text());

	if (ui.checkBox_hotKey->isChecked())
	{
		emit onHotKeyChanged(ui.lineEdit_mic_hotKey->text(),
			ui.lineEdit_live_hotKey->text(),
			ui.lineEdit_barrage_hotKey->text());
	}
	else
	{
		emit onHotKeyChanged("", "", "");
	}
}

void SettingsIniDlg::saveVideoDirSettings()
{
	if (!ui.lineEdit_dir->text().isEmpty())
	{
		config_set_string(main->Config(), "SimpleOutput", "FilePath", QT_TO_UTF8(ui.lineEdit_dir->text()));
		config_set_string(main->Config(), "AdvOut", "RecFilePath", QT_TO_UTF8(ui.lineEdit_dir->text()));
	}
}

void SettingsIniDlg::saveOtherSettings()
{
	int exitType = EXIT_TO_TRAY;
	if (ui.radioButton_exit->isChecked())
	{
		exitType = EXIT_APP;
	}
	UserConfig::instance()->setExitTipsType(exitType);

	g_dataCentre.m_stCurrentAccountInfo.autoLoginMark = ui.checkBox_autoLogin->isChecked();
	HttpAgent::instance()->RequestOnRecordUserInformation(g_dataCentre.m_stCurrentAccountInfo.qsAccountNumber, g_dataCentre.m_stCurrentAccountInfo.qsPassword,
		g_dataCentre.m_stCurrentAccountInfo.isRememberPwd, g_dataCentre.m_stCurrentAccountInfo.eLoginMethod, g_dataCentre.m_stCurrentAccountInfo.autoLoginMark);
}

void SettingsIniDlg::setCustomizeParams(bool isCustomized)
{
	ui.comboBox_bitrate_val->setDisabled(!isCustomized);
	ui.comboBox_frame_val->setDisabled(!isCustomized);
	ui.comboBox_resolution_val->setDisabled(!isCustomized);
}

void SettingsIniDlg::AddComboItem(QComboBox * combo, const char *text, const char *val)
{
	combo->addItem(QT_UTF8(text), QT_UTF8(val));
}

void SettingsIniDlg::AddComboItems(QComboBox *combo, obs_property_t *prop, int index)
{
	size_t count = obs_property_list_item_count(prop);

	obs_source_t *source = obs_get_output_source(index);
	const char *deviceId = nullptr;
	obs_data_t *settings = nullptr;

	if (source) {
		settings = obs_source_get_settings(source);
		if (settings) {
			deviceId = obs_data_get_string(settings, "device_id");
		}

		if (index == 3) {
			AddFilterForAudioSource("noise_suppress_filter", source);
			InitNoiseSuppress(source);
		}
	}
	combo->addItem(QTStr("Basic.Settings.Audio.Disabled"), "disabled");

	for (size_t i = 0; i < count; i++) {
		const char *name = obs_property_list_item_name(prop, i);
		const char *val  = obs_property_list_item_string(prop, i);
		AddComboItem(combo, name, val);
	}

	if (deviceId) {
		QVariant var(QT_UTF8(deviceId));
		auto idx = combo->findData(var);
		if (idx != -1) {
			combo->setCurrentIndex(idx);
		}
		else {
			combo->insertItem(0,
				QTStr("Basic.Settings.Audio."
					"UnknownAudioDevice"),
				var);
			combo->setCurrentIndex(0);
		}
	}

	if (settings)
		obs_data_release(settings);
	if (source)
		obs_source_release(source);
}

void SettingsIniDlg::RestrictResetBitrates(QComboBox *combo, int maxbitrate)
{
	int idx = combo->currentIndex();
	int max_bitrate = FindClosestAvailableAACBitrate(maxbitrate);
	int count = combo->count();
	int max_idx = combo->findText(QT_UTF8(std::to_string
	(max_bitrate).c_str()));

	for (int i = (count - 1); i > max_idx; i--)
		combo->removeItem(i);

	if (idx > max_idx) {
		int default_bitrate = FindClosestAvailableAACBitrate(
			maxbitrate / 2);
		int default_idx = combo->findText(QT_UTF8(std::to_string
		(default_bitrate).c_str()));

		combo->setCurrentIndex(default_idx);
		combo->setProperty("changed", QVariant(true));
	}
	else {
		combo->setCurrentIndex(idx);
	}
}

void SettingsIniDlg::AddFilterForAudioSource(const char * id, obs_source_t * source)
{
	if (id && *id) {
		obs_source_t *existing_filter;
		string name = obs_source_get_display_name(id);

		existing_filter = obs_source_get_filter_by_name(source, name.c_str());
		if (existing_filter) {
			obs_source_release(existing_filter);
			return;
		}

		obs_source_t *filter = obs_source_create(id, name.c_str(), nullptr, nullptr);
		if (filter) {
			const char *sourceName = obs_source_get_name(source);

			blog(LOG_INFO, "Default added filter '%s' (%s) "
				"to source '%s'",
				name.c_str(), id, sourceName);

			obs_source_filter_add(source, filter);
			obs_source_release(filter);
		}
	}
}

void SettingsIniDlg::InitNoiseSuppress(obs_source_t *source)
{
	string name              = obs_source_get_display_name("noise_suppress_filter");
	obs_source_t *filter     = obs_source_get_filter_by_name(source, name.c_str());
	m_pAudioSettings         = obs_source_get_settings(filter);

	obs_properties_t *prop   = obs_get_source_properties("noise_suppress_filter");
	obs_property_t *property = obs_properties_first(prop);
	if (property)
	{
		obs_number_type type = obs_property_int_type(property);
		const char *pname    = obs_property_name(property);
		int        val       = (int)obs_data_get_int(m_pAudioSettings, pname);

		int minVal           = obs_property_int_min(property);
		int maxVal           = obs_property_int_max(property);
		int stepVal          = obs_property_int_step(property);

		if (type == OBS_NUMBER_SLIDER) 
		{
			connect(ui.slider_denoise, &QSlider::valueChanged, [&](int value) {
				bool isHidden = true;
				if (value > -10) {
					isHidden = false;
				}

				ui.label_warning->setHidden(isHidden);
				ui.label_denoise_val->setNum(value);
			
				obs_data_set_int(m_pAudioSettings, "suppress_level", value);
			});

			ui.slider_denoise->setMinimum(minVal);
			ui.slider_denoise->setMaximum(maxVal);
			ui.slider_denoise->setPageStep(stepVal);
			ui.slider_denoise->setValue(val);
		}
	}

	obs_source_release(filter);
}
