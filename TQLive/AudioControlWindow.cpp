/***********************************************************************************
*								AudioControlWindow.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-11
*
* Description: 音频控制窗体
***********************************************************************************/
#include "AudioControlWindow.h"
#include "UserConfig.h"
#include "window-basic-main.hpp"
#include "window-basic-source-select.hpp"
#include "qt-wrappers.hpp"
#include "obs-app.hpp"

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

AudioControlWindow::AudioControlWindow(QWidget *parent)
	: QWidget(parent)
	, m_nCurr_MICValue(0)
	, m_nCurr_JBLValue(0)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	ui.slider_MIC->setMinimum(0);
	ui.slider_MIC->setMaximum(100);
	ui.slider_JBL->setMinimum(0);
	ui.slider_JBL->setMaximum(100);

	InitConenct();

	ui.slider_MIC->setValue(UserConfig::instance()->getCurMICValue());
	ui.slider_JBL->setValue(UserConfig::instance()->getCurJBLValue());
}

AudioControlWindow::~AudioControlWindow()
{

}

void AudioControlWindow::onSoundOffMIC()
{
	if (ui.button_MIC->isChecked())
	{
		m_nCurr_MICValue = ui.slider_MIC->value();
		ui.slider_MIC->setValue(0);
	}
	else
	{
		ui.slider_MIC->setValue(m_nCurr_MICValue);
	}
}

void AudioControlWindow::onSoundOffJBL()
{
	if (ui.button_JBL->isChecked())
	{
		m_nCurr_JBLValue = ui.slider_JBL->value();
		ui.slider_JBL->setValue(0);
	}
	else
	{
		ui.slider_JBL->setValue(m_nCurr_JBLValue);
	}
}

void AudioControlWindow::onMICValueChanged(int val)
{
	if (val == 0)
	{
		ui.button_MIC->setChecked(true);
	}
	else
	{
		ui.button_MIC->setChecked(false);
	}

	UserConfig::instance()->setCurMICValue(ui.slider_MIC->value());
	emit onSigMicValueChanged(ui.slider_MIC->value());
}

void AudioControlWindow::onJBLValueChanged(int val)
{
	if (val == 0)
	{
		ui.button_JBL->setChecked(true);
	}
	else
	{
		ui.button_JBL->setChecked(false);
	}

	UserConfig::instance()->setCurJBLValue(ui.slider_JBL->value());
	emit onSigJblValueChanged(ui.slider_JBL->value());
}

void AudioControlWindow::InitConenct()
{
	connect(ui.button_MIC, SIGNAL(clicked()), this, SLOT(onSoundOffMIC()));
	connect(ui.button_JBL, SIGNAL(clicked()), this, SLOT(onSoundOffJBL()));

	connect(ui.slider_MIC, SIGNAL(valueChanged(int)), this, SLOT(onMICValueChanged(int)));
	connect(ui.slider_JBL, SIGNAL(valueChanged(int)), this, SLOT(onJBLValueChanged(int)));
}

void AudioControlWindow::InitMicSource()
{
	OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	QString name = QStringLiteral("音频输入");
	obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
	if (source)
	{
		obs_source_release(source);
	}
	else
	{
		source = obs_source_create("wasapi_input_capture", name.toStdString().c_str(), NULL, nullptr);
		if (source)
		{
			AddSourceData data;
			data.source = source;
			data.visible = true;

			obs_enter_graphics();
			obs_scene_atomic_update(scene, AddSource, &data);
			obs_leave_graphics();

		}
	}
}

void AudioControlWindow::SetMicHotKey(QString micHotKey)
{
	ui.button_MIC->setShortcut(micHotKey);
}
