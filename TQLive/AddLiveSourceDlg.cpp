/***********************************************************************************
*								AddLiveSourceDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-11
*
* Description: 直播源选择对话框
***********************************************************************************/
#include "AddLiveSourceDlg.h"
#include "FramelessHelper.h"
#include "window-basic-main.hpp"
#include "qt-wrappers.hpp"

#include <QToolTip>

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

AddLiveSourceDlg::AddLiveSourceDlg(QWidget *parent, OBSSource source, bool initFlag)
	: QDialog(parent)
	, m_pSource(source)
	, m_bNeedInit(initFlag)
	, properties(nullptr, obs_properties_destroy)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	setModal(true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	main = reinterpret_cast<OBSBasic*>(parent);

	InitUI();
	InitConnect();
	InitSource();
}

AddLiveSourceDlg::~AddLiveSourceDlg()
{

}

void AddLiveSourceDlg::Load()
{
	if (m_pSource && input_property) {
		obs_data_t    *settings = obs_source_get_settings(m_pSource);
		const char    *name     = obs_property_name(input_property);
		const char    *val      = obs_data_get_string(settings, name);

		ui.lineEdit_liveSource->setText(QT_UTF8(val));
		ui.lineEdit_liveSource->setToolTip(QT_UTF8(obs_property_long_description(input_property)));

		obs_data_release(settings);
	}

	show();
}

QString AddLiveSourceDlg::getSourceName()
{
	QString sourceName;
	if (m_pSource)
		sourceName = QT_UTF8(obs_source_get_name(m_pSource));

	return sourceName;
}

void AddLiveSourceDlg::closeEvent(QCloseEvent * event)
{
	Q_UNUSED(event)

	deleteLater();
}

void AddLiveSourceDlg::onDialogClosed()
{
	if (m_bNeedInit)
	{
		OBSBasic *main = reinterpret_cast<OBSBasic*>(App()->GetMainWindow());
		main->CancelCurrentSource(getSourceName());
	}

	close();
}

void AddLiveSourceDlg::onConfirmClicked()
{
	QString path = ui.lineEdit_liveSource->text();
	QFont font;
	font.setFamily(QString("Microsoft YaHei"));
	font.setPixelSize(14);
	QToolTip::setFont(font);

	if (path.isEmpty()) {
		QToolTip::showText(ui.lineEdit_liveSource->mapToGlobal(QPoint(0, 0)), QStringLiteral("请输入直播源！"));
		return;
	}

	if (path.startsWith("http://") || path.startsWith("rtsp://")
		|| path.startsWith("rtmp://") || path.startsWith("https://")) {
		if (m_pSource && !path.contains(" ")) {
			obs_data_t *settings = obs_source_get_settings(m_pSource);
			obs_data_set_string(settings, "input", QT_TO_UTF8(path));
			obs_source_update(m_pSource, settings);
			obs_data_release(settings);

			main->SaveProject();
			m_bNeedInit = false;
			close();
			return;
		}
	}

	ui.lineEdit_liveSource->clear();
	ui.lineEdit_liveSource->setFocus();
	QToolTip::showText(ui.lineEdit_liveSource->mapToGlobal(QPoint(0, 0)), QStringLiteral("请输入正确的直播源！"));
}

void AddLiveSourceDlg::InitUI()
{
	ui.listWidget_liveSource->hide();
	ui.button_confirm->setCursor(Qt::PointingHandCursor);
	ui.button_cancel->setCursor(Qt::PointingHandCursor);
	ui.button_close->setCursor(Qt::PointingHandCursor);
}

void AddLiveSourceDlg::InitConnect()
{
	connect(ui.button_close,  SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
}

void AddLiveSourceDlg::InitSource()
{
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	int index = 1;
	if (!m_pSource) {
		while (true) {
			QString name = QStringLiteral("直播源%1").arg(index);
			obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
			if (source) {
				obs_source_release(source);
				index++;
			}
			else {
				source = obs_source_create("ffmpeg_source", name.toStdString().c_str(), NULL, nullptr);
				if (source) {
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

	properties.reset(obs_source_properties(m_pSource));
	obs_property_t *property = obs_properties_first(properties.get());
	while (property) {
		if (strcmp(obs_property_name(property), "input") == 0) {
			input_property = property;
		}
		obs_property_next(&property);
	}

	obs_data_t *settings = obs_source_get_settings(m_pSource);
	obs_data_set_bool(settings, "is_local_file", false);
	obs_data_release(settings);
}
