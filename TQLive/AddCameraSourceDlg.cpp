/***********************************************************************************
*								AddCameraSourceDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-10
*
* Description: 摄像头设置对话框
***********************************************************************************/
#include "AddCameraSourceDlg.h"
#include "FramelessHelper.h"
#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"
#include "CameraCacheJson.h"

#include <QColorDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPalette>
#include <string>

using namespace std;

struct AddSourceData {
	obs_source_t *source;
	bool visible;
};

static void AddSource(void *_data, obs_scene_t *scene)
{
	AddSourceData *data = (AddSourceData *)_data;
	obs_sceneitem_t *sceneitem;

	sceneitem = obs_scene_add(scene, data->source);
	obs_sceneitem_set_visible(sceneitem, data->visible);
}

template <long long get_int(obs_data_t*, const char*),
	double get_double(obs_data_t*, const char*),
	const char *get_string(obs_data_t*, const char*)>
	static string from_obs_data(obs_data_t *data, const char *name,
		obs_combo_format format)
{
	switch (format) {
	case OBS_COMBO_FORMAT_INT:
		return to_string(get_int(data, name));
	case OBS_COMBO_FORMAT_FLOAT:
		return to_string(get_double(data, name));
	case OBS_COMBO_FORMAT_STRING:
		return get_string(data, name);
	default:
		return "";
	}
}

static string from_obs_data(obs_data_t *data, const char *name,
	obs_combo_format format)
{
	return from_obs_data<obs_data_get_int, obs_data_get_double,
		obs_data_get_string>(data, name, format);
}

static string from_obs_data_autoselect(obs_data_t *data, const char *name,
	obs_combo_format format)
{
	return from_obs_data<obs_data_get_autoselect_int,
		obs_data_get_autoselect_double,
		obs_data_get_autoselect_string>(data, name, format);
}

AddCameraSourceDlg::AddCameraSourceDlg(QWidget *parent, OBSSource source, bool initFlag)
	: QDialog(parent)
	, m_bThinBodySwitch(false)
	, m_bWhitenPicSwitch(false)
	, m_bBgFilterSwitch(false)
	, m_bNeedInit(initFlag)
	, m_pSource(source)
	, properties(nullptr, obs_properties_destroy)
	, removeSignal(obs_source_get_signal_handler(source),
		"filter_remove",
		AddCameraSourceDlg::OBSSourceFilterRemoved,
		this)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	setModal(true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	ui.button_close->setCursor(Qt::PointingHandCursor);
	ui.button_confirm->setCursor(Qt::PointingHandCursor);
	ui.button_cancel->setCursor(Qt::PointingHandCursor);

	InitUI();
	InitConnect();
	InitSource();
}

AddCameraSourceDlg::~AddCameraSourceDlg()
{
	obs_data_t *settings = obs_source_get_settings(m_pSource);
	bool active = obs_data_get_bool(settings, "active");
	obs_data_release(settings);
	if (active_property && active) {
		obs_property_button_clicked(active_property, m_pSource);
	}
}

void AddCameraSourceDlg::Load()
{
	show();
}

void AddCameraSourceDlg::Release()
{
	int index = -1;
	bool isOperate = false;
	for (auto &iter : CameraCacheJson::instance()->m_pCameraCacheList) {
		index++;
		if (iter.sourceName == getSourceName()) {
			isOperate = true;
			break;
		}
	}

	if (index > -1 && isOperate) {
		CameraCacheJson::instance()->m_pCameraCacheList.removeAt(index);
		CameraCacheJson::instance()->Write();
	}
}

QString AddCameraSourceDlg::getSourceName()
{
	QString sourceName;
	if (m_pSource)
		sourceName = QT_UTF8(obs_source_get_name(m_pSource));

	return sourceName;
}

void AddCameraSourceDlg::onThinBodyValueChanged(int value)
{
	ui.label_thinBody_val->setNum(value);
}

void AddCameraSourceDlg::onWhitenPicValueChanged(int value)
{
	ui.label_whitenPic_val->setNum(value);
}

void AddCameraSourceDlg::onSimilarityValueChanged(int value)
{
	ui.label_similarity_val->setNum(value);

	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings = obs_source_get_settings(filter);
		obs_data_set_int(settings, "similarity", value);
		obs_source_update(filter, settings);
		obs_data_release(settings);
	}
}

void AddCameraSourceDlg::onSmoothnessValueChanged(int value)
{
	ui.label_smoothness_val->setNum(value);

	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings = obs_source_get_settings(filter);
		obs_data_set_int(settings, "smoothness", value);
		obs_source_update(filter, settings);
		obs_data_release(settings);
	}
}

void AddCameraSourceDlg::onSpillValueChanged(int value)
{
	ui.label_spill_val->setNum(value);

	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings = obs_source_get_settings(filter);
		obs_data_set_int(settings, "spill", value);
		obs_source_update(filter, settings);
		obs_data_release(settings);
	}
}

void AddCameraSourceDlg::onCameraSelectedIndexChanged(int index)
{
	QVariant data;
	if (index != -1)
		data = ui.comboBox_cameraSelected->itemData(index);

	obs_data_t *settings = obs_source_get_settings(m_pSource);
	obs_data_set_string(settings, "video_device_id",
		data.toByteArray().constData());
	obs_property_modified(dshow_property, settings);
	obs_data_release(settings);
}

void AddCameraSourceDlg::onInputResolutionIndexChanged(int index)
{
	if (index == -1)
		return;

	QVariant  data = ui.comboBox_inputResolution->itemData(index);
	if (m_pSource && m_bIsActive) {
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		if (data.toString() == "-1") {
			obs_data_set_int(settings, "res_type", 0);
		}
		else {
			obs_data_set_int(settings, "res_type", 1);
			obs_data_set_string(settings, "resolution", data.toByteArray().constData());
			obs_data_set_int(settings, "frame_interval", 333333);
			obs_data_set_int(settings, "video_format", 400);
		}
		obs_property_modified(output_property, settings);
		obs_source_update(m_pSource, settings);
		obs_data_release(settings);
	}
} 

void AddCameraSourceDlg::onThinBodySwitchStateChanged(int state)
{

}

void AddCameraSourceDlg::onWhitenPicSwitchStateChanged(int state)
{

}

void AddCameraSourceDlg::onBgFilterSwitchStateChanged(int state)
{
	if (state == Qt::Checked) {
		AddNewFilter("chroma_key_filter");
		LoadBackgroundFilter();
	}
	else {
		RemoveFilter();
	}
}

void AddCameraSourceDlg::onFlipHorizontalStateChanged(int state)
{
	emit onSigFlipHorizontal();
}

void AddCameraSourceDlg::onFlipVerticalStateChanged(int state)
{
	if (m_pSource) {
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		obs_data_set_bool(settings, "flip_vertically", state);
		obs_source_update(m_pSource, settings);
		obs_data_release(settings);
	}
}

void AddCameraSourceDlg::onOpenCamera()
{
	if (active_property && !m_bIsActive) {
		obs_property_button_clicked(active_property, m_pSource);

		obs_data_t *settings = obs_source_get_settings(m_pSource);
		m_bIsActive = obs_data_get_bool(settings, "active");
		obs_data_release(settings);

		ui.button_cameraOpened->setDisabled(m_bIsActive);
		ui.comboBox_cameraSelected->setDisabled(m_bIsActive);
		onInputResolutionIndexChanged(ui.comboBox_inputResolution->currentIndex());
	}
}

void AddCameraSourceDlg::onPickColorRGB()
{
	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings = obs_source_get_settings(filter);
		long long  val       = obs_data_get_int(settings, "key_color");
		QColor     color     = color_from_int(val);

		QColorDialog::ColorDialogOptions options =
			QColorDialog::ShowAlphaChannel;

		color = QColorDialog::getColor(color, NULL, QStringLiteral("选择颜色"), options);
		color.setAlpha(255);

		if (!color.isValid())
			return;

		QPalette palette = QPalette(color);
		ui.button_color_val->setPalette(palette);
		ui.button_color_val->setStyleSheet(QString("background-color: %1; border: none;")
		                                 .arg(palette.color(QPalette::Window).name(QColor::HexArgb)));

		obs_data_set_int(settings, "key_color", color_to_int(color));
		obs_data_release(settings);
	}	
}

void AddCameraSourceDlg::onRebackSettings()
{
	if (ui.comboBox_cameraSelected->currentIndex() != -1)
		ui.comboBox_cameraSelected->setCurrentIndex(0);

	int idx = ui.comboBox_inputResolution->findData("-1");
	if (idx != -1)
		ui.comboBox_inputResolution->setCurrentIndex(idx);

	ui.checkBox_thinBody->setChecked(false);
	ui.checkBox_whitenPic->setChecked(false);

	ui.horizontalSlider_similarity->setValue(1);
	ui.horizontalSlider_smoothness->setValue(1);
	ui.horizontalSlider_spill->setValue(1);

	string name = obs_source_get_display_name("chroma_key_filter");
	OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
	obs_data_t *settings = obs_source_get_settings(filter);
	QPalette palette = QPalette(Qt::green);
	ui.button_color_val->setPalette(palette);
	ui.button_color_val->setStyleSheet(QString("background-color: %1; border: none;")
		.arg(palette.color(QPalette::Window).name(QColor::HexArgb)));
	obs_data_set_int(settings, "key_color", color_to_int(Qt::green));
	obs_data_release(settings);

	ui.checkBox_background_filter->setChecked(false);
	ui.checkBox_flipHorizontal->setChecked(false);
	ui.checkBox_flipVertical->setChecked(false);

	m_cacheData.cameraItemIdx = ui.comboBox_cameraSelected->currentIndex();
	m_cacheData.resoultionItemIdx = ui.comboBox_inputResolution->currentIndex();
	m_cacheData.filterOfBgSelected = false;
	m_cacheData.similarityVal = 1;
	m_cacheData.smoothnessVal = 1;
	m_cacheData.spillVal = 1;
	m_cacheData.filterOfBgColor = color_to_int(Qt::green);
	m_cacheData.flipTypeVal = 0;

	SaveCameraCache();
}

void AddCameraSourceDlg::onConfirmClicked()
{
	SaveBasicSettings();
	SaveSmartbeautification();
	SaveBackgroundFilter();
	SaveDirectionOfLens();
	SaveCameraCache();

	m_bNeedInit = false;
	this->close();
}

void AddCameraSourceDlg::onCancelClicked()
{
	if (m_bNeedInit) {
		OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
		main->CancelCurrentSource(getSourceName());
	}
	else {
		ui.comboBox_cameraSelected->setCurrentIndex(m_cacheData.cameraItemIdx);
		ui.comboBox_inputResolution->setCurrentIndex(m_cacheData.resoultionItemIdx);
		ui.checkBox_background_filter->setChecked(m_cacheData.filterOfBgSelected);

		string name = obs_source_get_display_name("chroma_key_filter");
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		obs_data_t *settings = obs_source_get_settings(filter);
		QPalette palette = QPalette(color_from_int(m_cacheData.filterOfBgColor));
		ui.button_color_val->setPalette(palette);
		ui.button_color_val->setStyleSheet(QString("background-color: %1; border: none;")
			.arg(palette.color(QPalette::Window).name(QColor::HexArgb)));
		obs_data_set_int(settings, "key_color", m_cacheData.filterOfBgColor);
		obs_data_release(settings);

		ui.horizontalSlider_similarity->setValue(m_cacheData.similarityVal);
		ui.horizontalSlider_smoothness->setValue(m_cacheData.smoothnessVal);
		ui.horizontalSlider_spill->setValue(m_cacheData.spillVal);

		switch (m_cacheData.flipTypeVal)
		{
		case 0:
			ui.checkBox_flipHorizontal->setChecked(false);
			ui.checkBox_flipVertical->setChecked(false);
			break;
		case 1:
			ui.checkBox_flipHorizontal->setChecked(true);
			ui.checkBox_flipVertical->setChecked(false);
			break;
		case 2:
			ui.checkBox_flipHorizontal->setChecked(false);
			ui.checkBox_flipVertical->setChecked(true);
			break;
		case 3:
			ui.checkBox_flipHorizontal->setChecked(true);
			ui.checkBox_flipVertical->setChecked(true);
			break;
		default:
			break;
		}
	}

	this->reject();
}

void AddCameraSourceDlg::InitUI()
{
	InitBasicSettings();
	InitSmartbeautification();
	InitBackgroundFilter();
	InitDirectionOfLens();
}

void AddCameraSourceDlg::InitConnect()
{
	connect(ui.button_close, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
	connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
	connect(ui.button_cameraOpened, SIGNAL(clicked()), this, SLOT(onOpenCamera()));
	connect(ui.button_color_val, SIGNAL(clicked()), this, SLOT(onPickColorRGB()));
	connect(ui.button_restoreDefault, SIGNAL(clicked()), this, SLOT(onRebackSettings()));
	connect(ui.button_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));

	connect(ui.horizontalSlider_thinBody, SIGNAL(valueChanged(int)), this, SLOT(onThinBodyValueChanged(int)));
	connect(ui.horizontalSlider_whitenPic, SIGNAL(valueChanged(int)), this, SLOT(onWhitenPicValueChanged(int)));
	connect(ui.horizontalSlider_similarity, SIGNAL(valueChanged(int)), this, SLOT(onSimilarityValueChanged(int)));
	connect(ui.horizontalSlider_smoothness, SIGNAL(valueChanged(int)), this, SLOT(onSmoothnessValueChanged(int)));
	connect(ui.horizontalSlider_spill, SIGNAL(valueChanged(int)), this, SLOT(onSpillValueChanged(int)));

 	//connect(ui.comboBox_cameraSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(onCameraSelectedIndexChanged(int)));
 	//connect(ui.comboBox_inputResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputResolutionIndexChanged(int)));

	connect(ui.checkBox_thinBody, SIGNAL(stateChanged(int)), this, SLOT(onThinBodySwitchStateChanged(int)));
	connect(ui.checkBox_whitenPic, SIGNAL(stateChanged(int)), this, SLOT(onWhitenPicSwitchStateChanged(int)));
	connect(ui.checkBox_background_filter, SIGNAL(stateChanged(int)), this, SLOT(onBgFilterSwitchStateChanged(int)));
	connect(ui.checkBox_flipHorizontal, SIGNAL(stateChanged(int)), this, SLOT(onFlipHorizontalStateChanged(int)));
	connect(ui.checkBox_flipVertical, SIGNAL(stateChanged(int)), this, SLOT(onFlipVerticalStateChanged(int)));
}

void AddCameraSourceDlg::InitSource()
{
	OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	if (!m_pSource) {
		int index = 1;
		while (true)
		{
			QString name = QStringLiteral("摄像头%1").arg(index);
			obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
			if (source)
			{
				obs_source_release(source);
				index++;
			}
			else
			{
				source = obs_source_create("dshow_input", name.toStdString().c_str(), NULL, nullptr);
				if (source)
				{
					AddSourceData data;
					data.source = source;
					data.visible = true;

					obs_enter_graphics();
					obs_scene_atomic_update(scene, AddSource, &data);
					obs_leave_graphics();

					m_pSource = source;
				}
				break;
			}
		}
	}

	ReloadProperties();
	ReloadCache();
	ReloadIni();
}

void AddCameraSourceDlg::InitBasicSettings()
{
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,1920*1080,30fps"), QVariant::fromValue<QString>("1920x1080"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,1600*896,30fps"), QVariant::fromValue<QString>("1600x896"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,1280*720,30fps"), QVariant::fromValue<QString>("1280x720"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,1024*576,30fps"), QVariant::fromValue<QString>("1024x576"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,960*720,30fps"), QVariant::fromValue<QString>("960x720"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,864*480,30fps"), QVariant::fromValue<QString>("864x480"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,800*600,30fps"), QVariant::fromValue<QString>("800x600"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,800*448,30fps"), QVariant::fromValue<QString>("800x448"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,640*360,30fps"), QVariant::fromValue<QString>("640x360"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,640*480,30fps"), QVariant::fromValue<QString>("640x480"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,432*240,30fps"), QVariant::fromValue<QString>("432x240"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,352*288,30fps"), QVariant::fromValue<QString>("352x288"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,320*240,30fps"), QVariant::fromValue<QString>("320x240"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,320*180,30fps"), QVariant::fromValue<QString>("320x180"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,176*144,30fps"), QVariant::fromValue<QString>("176x144"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,160*120,30fps"), QVariant::fromValue<QString>("160x120"));
// 	ui.comboBox_inputResolution->addItem(QT_UTF8("MJPG,160*90,30fps"), QVariant::fromValue<QString>("160x90"));
// 	ui.comboBox_inputResolution->addItem(QStringLiteral("默认"), QVariant::fromValue<QString>("-1"));

	ui.button_cameraOpened->setCursor(Qt::PointingHandCursor);
}

void AddCameraSourceDlg::InitSmartbeautification()
{
	ui.horizontalSlider_thinBody->setMinimum(-50);
	ui.horizontalSlider_thinBody->setMaximum(50);
	ui.horizontalSlider_thinBody->setSingleStep(1);
	ui.horizontalSlider_thinBody->setValue(0);

	ui.horizontalSlider_whitenPic->setMinimum(0);
	ui.horizontalSlider_whitenPic->setMaximum(100);
	ui.horizontalSlider_whitenPic->setSingleStep(1);
	ui.horizontalSlider_whitenPic->setValue(0);

	ui.checkBox_thinBody->setCursor(Qt::PointingHandCursor);
	ui.checkBox_whitenPic->setCursor(Qt::PointingHandCursor);
	ui.wgt_smart_beautification->setHidden(true);
}

void AddCameraSourceDlg::InitBackgroundFilter()
{
	ui.horizontalSlider_similarity->setMinimum(1);
	ui.horizontalSlider_similarity->setMaximum(1000);
	ui.horizontalSlider_similarity->setSingleStep(1);
	ui.horizontalSlider_similarity->setValue(1);

	ui.horizontalSlider_smoothness->setMinimum(1);
	ui.horizontalSlider_smoothness->setMaximum(1000);
	ui.horizontalSlider_smoothness->setSingleStep(1);
	ui.horizontalSlider_smoothness->setValue(1);

	ui.horizontalSlider_spill->setMinimum(1);
	ui.horizontalSlider_spill->setMaximum(1000);
	ui.horizontalSlider_spill->setSingleStep(1);
	ui.horizontalSlider_spill->setValue(1);

	ui.button_color_val->setCursor(Qt::PointingHandCursor);
	ui.checkBox_background_filter->setCursor(Qt::PointingHandCursor);

	QPalette palette = QPalette(Qt::green);
	ui.button_color_val->setPalette(palette);
	ui.button_color_val->setStyleSheet(QString("background-color :%1; border: none;")
		.arg(palette.color(QPalette::Window).name(QColor::HexArgb)));
}

void AddCameraSourceDlg::InitDirectionOfLens()
{
	ui.checkBox_flipHorizontal->setCursor(Qt::PointingHandCursor);
	ui.checkBox_flipVertical->setCursor(Qt::PointingHandCursor);
}

void AddCameraSourceDlg::ReloadProperties()
{
	properties.reset(obs_source_properties(m_pSource));
	obs_property_t *property = obs_properties_first(properties.get());
	while (property) {
		if (strcmp(obs_property_name(property), "video_device_id") == 0) {
			dshow_property = property;
		}
		else if (strcmp(obs_property_name(property), "activate") == 0) {
			active_property = property;
		}
		else if (strcmp(obs_property_name(property), "resolution") == 0) {
			output_property = property;
		}
		else if (strcmp(obs_property_name(property), "flip_vertically") == 0) {
			flip_property = property;
		}
		obs_property_next(&property);
	}
}

void AddCameraSourceDlg::ReloadCache()
{
	QString name = QT_UTF8(obs_source_get_name(m_pSource));
	for (auto &iter : CameraCacheJson::instance()->m_pCameraCacheList) {
		if (iter.sourceName == name) {
			m_cacheData = iter;
			break;
		}
	}
}

void AddCameraSourceDlg::ReloadIni()
{
	LoadBasicSettings();
	LoadSmartbeautification();
	LoadBackgroundFilter();
	LoadDirectionOfLens();
}

void AddCameraSourceDlg::LoadBasicSettings()
{
	LoadCameraSelectedItems();
	LoadOpenCameraButton();
	LoadOuputResolution();
}

void AddCameraSourceDlg::LoadSmartbeautification()
{
	
}

void AddCameraSourceDlg::LoadBackgroundFilter()
{
	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings     = obs_source_get_settings(filter);
		long long        val     = obs_data_get_int(settings, "key_color");
		QColor         color     = color_from_int(val);
		int    similarityVal     = (int)obs_data_get_int(settings, "similarity");
		int    smoothnessVal     = (int)obs_data_get_int(settings, "smoothness");
		int         spillVal     = (int)obs_data_get_int(settings, "spill");

		color.setAlpha(255);
		QPalette palette = QPalette(color);
		ui.button_color_val->setPalette(palette);
		ui.button_color_val->setStyleSheet(QString("background-color :%1; border: none;")
		                     .arg(palette.color(QPalette::Window).name(QColor::HexArgb)));
		obs_data_set_int(settings, "key_color", color_to_int(color));
		obs_data_release(settings);

		ui.horizontalSlider_similarity->setValue(similarityVal);
		ui.horizontalSlider_smoothness->setValue(smoothnessVal);
		ui.horizontalSlider_spill->setValue(spillVal);
		ui.checkBox_background_filter->setChecked(true);
	}
}

void AddCameraSourceDlg::LoadDirectionOfLens()
{
	if (flip_property) {
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		bool val = obs_data_get_bool(settings, "flip_vertically");
		obs_data_release(settings);
		ui.checkBox_flipVertical->setCheckState(val ? Qt::Checked : Qt::Unchecked);
		ui.checkBox_flipHorizontal->setCheckState(m_cacheData.filterOfBgSelected ? Qt::Checked : Qt::Unchecked);
	}
}

void AddCameraSourceDlg::SaveBasicSettings()
{
	m_cacheData.cameraItemIdx = ui.comboBox_cameraSelected->currentIndex();
	m_cacheData.resoultionItemIdx = ui.comboBox_inputResolution->currentIndex();
}

void AddCameraSourceDlg::SaveSmartbeautification()
{

}

void AddCameraSourceDlg::SaveBackgroundFilter()
{
	if (ui.checkBox_background_filter->isChecked())
	{
		m_cacheData.filterOfBgSelected = true;
		m_cacheData.similarityVal = ui.horizontalSlider_similarity->value();
		m_cacheData.smoothnessVal = ui.horizontalSlider_smoothness->value();
		m_cacheData.spillVal = ui.horizontalSlider_spill->value();

		string name = obs_source_get_display_name("chroma_key_filter");
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		obs_data_t *settings = obs_source_get_settings(filter);
		long long        val = obs_data_get_int(settings, "key_color");
		m_cacheData.filterOfBgColor = val;
		obs_data_release(settings);
	}
	else
	{
		m_cacheData.filterOfBgSelected = false;
		m_cacheData.similarityVal = 1;
		m_cacheData.smoothnessVal = 1;
		m_cacheData.spillVal = 1;
		m_cacheData.filterOfBgColor = color_to_int(Qt::green);
	}
}

void AddCameraSourceDlg::SaveDirectionOfLens()
{
	if (ui.checkBox_flipHorizontal->isChecked() && ui.checkBox_flipVertical->isChecked())
		m_cacheData.flipTypeVal = 3;
	else if (ui.checkBox_flipHorizontal->isChecked() && !ui.checkBox_flipVertical->isChecked())
		m_cacheData.flipTypeVal = 1;
	else if (!ui.checkBox_flipHorizontal->isChecked() && ui.checkBox_flipVertical->isChecked())
		m_cacheData.flipTypeVal = 2;
	else
		m_cacheData.flipTypeVal = 0;
}

void AddCameraSourceDlg::SaveCameraCache()
{
	int index = -1;
	m_cacheData.sourceName = getSourceName();
	for (auto &iter : CameraCacheJson::instance()->m_pCameraCacheList) {
		if (iter.sourceName == m_cacheData.sourceName) {
			iter = m_cacheData;
			index++;
		}
	}

	if (index == -1) {
		CameraCacheJson::instance()->m_pCameraCacheList.append(m_cacheData);
	}

	CameraCacheJson::instance()->Write();
}

void AddCameraSourceDlg::AddCameraSelectedItem(obs_property_t * prop, size_t idx)
{
	const char* name = obs_property_list_item_name(prop, idx);
	QVariant var = QByteArray(obs_property_list_item_string(prop, idx));

	ui.comboBox_cameraSelected->addItem(QT_UTF8(name), var);

	if (!obs_property_list_item_disabled(prop, idx))
		return;

	int index = ui.comboBox_cameraSelected->findText(QT_UTF8(name));
	if (index < 0)
		return;

	QStandardItemModel *model =
		dynamic_cast<QStandardItemModel*>(ui.comboBox_cameraSelected->model());
	if (!model)
		return;

	QStandardItem *item = model->item(index);
	item->setFlags(Qt::NoItemFlags);
}

void AddCameraSourceDlg::LoadCameraSelectedItems()
{
	if (dshow_property) {
		const char* name = obs_property_name(dshow_property);
		obs_combo_type   type = obs_property_list_type(dshow_property);
		obs_combo_format format = obs_property_list_format(dshow_property);
		size_t           count = obs_property_list_item_count(dshow_property);
		int              idx = -1;
		bool             warning = false;

		for (size_t i = 0; i < count; i++)
			AddCameraSelectedItem(dshow_property, i);

		if (type == OBS_COMBO_TYPE_EDITABLE)
			ui.comboBox_cameraSelected->setEditable(true);

		ui.comboBox_cameraSelected->setMaxVisibleItems(40);
		ui.comboBox_cameraSelected->setToolTip(QT_UTF8(obs_property_long_description(dshow_property)));

		connect(ui.comboBox_cameraSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(onCameraSelectedIndexChanged(int)));
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		string value = from_obs_data(settings, name, format);
		obs_data_release(settings);
		idx = ui.comboBox_cameraSelected->findData(QByteArray(value.c_str()));

		if (idx != -1)
			ui.comboBox_cameraSelected->setCurrentIndex(idx);
		else
			onCameraSelectedIndexChanged(0);

		QAbstractItemModel *model = ui.comboBox_cameraSelected->model();
		warning = idx != -1 &&
			model->flags(model->index(idx, 0)) == Qt::NoItemFlags;
	}
}

static void AddComboItem(QComboBox *combo, obs_property_t *prop, size_t idx)
{
	const char* name = obs_property_list_item_name(prop, idx);
	QVariant var = QByteArray(obs_property_list_item_string(prop, idx));

	combo->addItem(QT_UTF8(name), var);

	if (!obs_property_list_item_disabled(prop, idx))
		return;

	int index = combo->findText(QT_UTF8(name));
	if (index < 0)
		return;

	QStandardItemModel *model =
		dynamic_cast<QStandardItemModel*>(combo->model());
	if (!model)
		return;

	QStandardItem *item = model->item(index);
	item->setFlags(Qt::NoItemFlags);
}

void AddCameraSourceDlg::LoadOpenCameraButton()
{
	if (active_property && m_pSource && m_bNeedInit) {
		obs_property_button_clicked(active_property, m_pSource);
	}

	obs_data_t *settings = obs_source_get_settings(m_pSource);
	m_bIsActive = obs_data_get_bool(settings, "active");

	obs_data_release(settings);
	ui.button_cameraOpened->setDisabled(m_bIsActive);
	ui.comboBox_cameraSelected->setDisabled(m_bIsActive);
}

void AddCameraSourceDlg::LoadOuputResolution()
{
	if (m_pSource) {
		int idx = -1;
		string value;
		const char       *name     = obs_property_name(output_property);
		obs_combo_format  format   = obs_property_list_format(output_property);
		obs_data_t       *settings = obs_source_get_settings(m_pSource);
		int                   type = obs_data_get_int(settings, "res_type");
		size_t               count = obs_property_list_item_count(output_property);

		for (size_t i = 0; i < count; i++)
			AddComboItem(ui.comboBox_inputResolution, output_property, i);
		ui.comboBox_inputResolution->addItem(QStringLiteral("默认"), QVariant::fromValue<QString>("-1"));
		connect(ui.comboBox_inputResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputResolutionIndexChanged(int)));

		if (type == 0) {
			value = "-1";
		}
		else if (type == 1) {
			value = from_obs_data(settings, name, format);
		}

		obs_data_release(settings);
		idx = ui.comboBox_inputResolution->findData(value.c_str());

		if (idx != -1)
			ui.comboBox_inputResolution->setCurrentIndex(idx);
	}
}

void AddCameraSourceDlg::AddNewFilter(const char* id)
{
	if (id && *id) {
		obs_source_t *existing_filter;
		string name = obs_source_get_display_name(id);
		if (!name.empty() && m_pSource) {
			existing_filter = obs_source_get_filter_by_name(m_pSource,
				name.c_str());
			if (existing_filter) {
				obs_source_release(existing_filter);
				return;
			}

			obs_source_t *filter = obs_source_create(id, name.c_str(), nullptr, nullptr);
			if (filter) {
				const char *sourceName = obs_source_get_name(m_pSource);

				blog(LOG_INFO, "User added filter '%s' (%s) "
					"to source '%s'",
					name.c_str(), id, sourceName);

				obs_source_filter_add(m_pSource, filter);

				obs_data_t *settings = obs_source_get_settings(filter);
				obs_data_set_string(settings, "key_color_type", "custom");
				obs_source_update(filter, settings);

				obs_data_release(settings);
				obs_source_release(filter);
			}
		}
	}
}

void AddCameraSourceDlg::RemoveFilter()
{
	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_source_filter_remove(m_pSource, filter);
	}
}

void AddCameraSourceDlg::RemoveFilter(OBSSource filter)
{
	const char *filterName = obs_source_get_name(filter);
	const char *sourceName = obs_source_get_name(m_pSource);
	if (!sourceName || !filterName)
		return;

	const char *filterId = obs_source_get_id(filter);
	blog(LOG_INFO, "User removed filter '%s' (%s) from source '%s'",
		filterName, filterId, sourceName);

	OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
	main->SaveProject();
}

void AddCameraSourceDlg::OBSSourceFilterRemoved(void *param, calldata_t *data)
{
	AddCameraSourceDlg *window = reinterpret_cast<AddCameraSourceDlg*>(param);
	obs_source_t *filter = (obs_source_t*)calldata_ptr(data, "filter");

	QMetaObject::invokeMethod(window, "RemoveFilter",
		Q_ARG(OBSSource, OBSSource(filter)));
}
