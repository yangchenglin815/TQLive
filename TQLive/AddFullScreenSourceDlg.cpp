/***********************************************************************************
*								AddFullScreenSourceDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-12
*
* Description: 添加全屏源对话框
***********************************************************************************/
#include "AddFullScreenSourceDlg.h"
#include "FramelessHelper.h"
#include "window-basic-main.hpp"
#include "display-helpers.hpp"
#include "qt-wrappers.hpp"
#include "obs-app.hpp"

#include <QStandardItemModel>
#include <QDebug>

using namespace std;

Q_DECLARE_METATYPE(OBSSource);

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


AddFullScreenSourceDlg::AddFullScreenSourceDlg(QWidget *parent)
	: QDialog(parent)
	, properties(nullptr, obs_properties_destroy)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
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

AddFullScreenSourceDlg::~AddFullScreenSourceDlg()
{

}

void AddFullScreenSourceDlg::closeEvent(QCloseEvent * event)
{
	Q_UNUSED(event)

	deleteLater();
}

static void AddComboItem(QComboBox *combo, obs_property_t *prop,
	obs_combo_format format, size_t idx)
{
	const char *name  = obs_property_list_item_name(prop, idx);
	long long   value = obs_property_list_item_int(prop, idx);
	QVariant    var   = QVariant::fromValue<long long>(value);

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

void AddFullScreenSourceDlg::Load()
{
	if (m_pSource && monitor_property) {
		const char       *name   = obs_property_name(monitor_property);
		obs_combo_format  format = obs_property_list_format(monitor_property);
		size_t            count  = obs_property_list_item_count(monitor_property);
		int               idx    = -1;

		for (size_t i = 0; i < count; i++)
			AddComboItem(ui.comboBox_selected, monitor_property, format, i);

		ui.comboBox_selected->setMaxVisibleItems(40);
		ui.comboBox_selected->setToolTip(QT_UTF8(obs_property_long_description(monitor_property)));

		obs_data_t *settings = obs_source_get_settings(m_pSource);
		string value = from_obs_data(settings, name, format);
		idx = ui.comboBox_selected->findData(QByteArray(value.c_str()));
		obs_data_release(settings);

		if (idx != -1)
			ui.comboBox_selected->setCurrentIndex(idx);

		if (idx == -1)
			onSelectedChanged(idx);
	}

	show();
}

QString AddFullScreenSourceDlg::getSourceName()
{
	QString sourceName;
	if (m_pSource) {
		sourceName = QT_UTF8(obs_source_get_name(m_pSource));
	}
	return sourceName;
}

void AddFullScreenSourceDlg::onDialogClosed()
{
	main->CancelCurrentSource(getSourceName());
	close();
}

void AddFullScreenSourceDlg::onConfirmClicked()
{
	main->SaveProject();
	close();
}

void AddFullScreenSourceDlg::onSelectedChanged(int index)
{
	QVariant data;

	if (index != -1)
		data = ui.comboBox_selected->itemData(index);
	else
		return;

	if (m_pSource) {
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		obs_data_set_int(settings, "monitor", data.value<long long>());
		obs_source_update(m_pSource, settings);
		obs_data_release(settings);
	}
}

void AddFullScreenSourceDlg::InitUI()
{
	ui.button_close->setCursor(Qt::PointingHandCursor);
	ui.button_cancel->setCursor(Qt::PointingHandCursor);
	ui.button_confirm->setCursor(Qt::PointingHandCursor);

	ui.comboBox_selected->setView(new QListView());
}

void AddFullScreenSourceDlg::InitConnect()
{
	connect(ui.button_close, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
	connect(ui.comboBox_selected, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectedChanged(int)));
}

void AddFullScreenSourceDlg::InitSource()
{
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	if (!m_pSource) {
		int index = 1;
		while (true)
		{
			QString name = QStringLiteral("全屏%1").arg(index);
			obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
			if (source)
			{
				obs_source_release(source);
				index++;
			}
			else
			{
				source = obs_source_create("monitor_capture", name.toStdString().c_str(), NULL, nullptr);
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

	properties.reset(obs_source_properties(m_pSource));
	obs_property_t *property = obs_properties_first(properties.get());
	while (property) {
		if (strcmp(obs_property_name(property), "monitor") == 0) {
			monitor_property = property;
		}
		obs_property_next(&property);
	}
}
