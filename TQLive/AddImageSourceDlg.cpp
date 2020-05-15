/***********************************************************************************
*								AddImageSourceDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-12
*
* Description: 添加图片源对话框
***********************************************************************************/
#include "AddImageSourceDlg.h"
#include "FramelessHelper.h"
#include "window-basic-main.hpp"
#include "display-helpers.hpp"
#include "qt-wrappers.hpp"
#include "obs-app.hpp"
#include "OpacityButton.h"

#include <QFileDialog>
#include <QGraphicsOpacityEffect>
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

AddImageSourceDlg::AddImageSourceDlg(QWidget *parent, OBSSource source, bool initFlag, QString path)
	: QDialog(parent)
	, m_pSource(source)
	, m_bNeedInit(initFlag)
	, properties(nullptr, obs_properties_destroy)
	, m_pSelectedButton(new OpacityButton(QStringLiteral("点击更改图片"), this))
	, m_ImgPath(path)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	setModal(true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

// 	auto addDrawCallback = [this]()
// 	{
// 		obs_display_add_draw_callback(ui.wgt_preview->GetDisplay(),
// 			AddImageSourceDlg::DrawPreview, this);
// 	};
// 	connect(ui.wgt_preview, &OBSQTDisplay::DisplayCreated,
// 		addDrawCallback);


	main = reinterpret_cast<OBSBasic*>(parent);

	InitUI();
	InitConnect();
	InitSource();
}

AddImageSourceDlg::~AddImageSourceDlg()
{
// 	obs_display_remove_draw_callback(ui.wgt_preview->GetDisplay(),
// 		AddImageSourceDlg::DrawPreview, this);
// 	obs_source_dec_showing(m_pSource
	main->SaveProject();
}

void AddImageSourceDlg::Load()
{
	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings = obs_source_get_settings(filter);
		int val = (int)obs_data_get_int(settings, "opacity");
		ui.slider_opacity->setValue(val);
		onOpacityValChanged(val);
		obs_data_release(settings);
	}

	show();
}

void AddImageSourceDlg::showEvent(QShowEvent * event)
{
	Q_UNUSED(event)

	int x, y;
	if (this->pos().x() == 0 && this->pos().y() == 0) 
	{
		x = main->pos().x() + (main->width() - this->width()) / 2
			+ ui.wgt_preview->mapToGlobal(this->pos()).x();
		y = main->pos().y() + (main->height() - this->height()) / 2
			+ ui.wgt_preview->mapToGlobal(this->pos()).y()
			+ (ui.wgt_preview->height() - 48);
	}
	else 
	{
		x = this->pos().x() + ui.wgt_preview->pos().x();
		y = this->pos().y() + ui.wgt_preview->pos().y() 
			+ (ui.wgt_preview->height() - 48);
	}

	m_pSelectedButton->setFixedSize(ui.wgt_preview->width(), 48);
	m_pSelectedButton->setCursor(Qt::PointingHandCursor);
	m_pSelectedButton->move(x, y);
	m_pSelectedButton->show();
}

void AddImageSourceDlg::moveEvent(QMoveEvent *event)
{
	Q_UNUSED(event)

	int x = event->pos().x() + ui.wgt_preview->pos().x();
	int y = event->pos().y() + ui.wgt_top->height() + 
		(ui.wgt_preview->height() - m_pSelectedButton->height()) + 21;

	if (m_pSelectedButton) {
		m_pSelectedButton->move(x, y);
	}
}

QString AddImageSourceDlg::getSourceName()
{
	QString sourceName;
	if (m_pSource) {
		sourceName = QT_UTF8(obs_source_get_name(m_pSource));
	}
	return sourceName;
}

void AddImageSourceDlg::Release()
{

}

void AddImageSourceDlg::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event)

	m_pSelectedButton->hide();
}

void AddImageSourceDlg::onConfirmClicked()
{
	m_bNeedInit = false;
	main->SaveProject();
	this->close();
}

void AddImageSourceDlg::onOpenImage()
{
	if (file_property && m_pSource) {
		const char    *desc = obs_property_description(file_property);
		obs_path_type type = obs_property_path_type(file_property);
		const char    *filter = obs_property_path_filter(file_property);
		const char    *default_path = obs_property_path_default_path(file_property);
		QString       path;

		if (type == OBS_PATH_FILE) {
			path = QFileDialog::getOpenFileName(NULL,
				QT_UTF8(desc), QT_UTF8(default_path),
				QT_UTF8(filter));
		}

		if (path.isEmpty())
			return;

		obs_data_t *settings = obs_source_get_settings(m_pSource);
		if (!settings)
			return;

		obs_data_set_string(settings, "file", QT_TO_UTF8(path));
		obs_source_update(m_pSource, settings);
		obs_data_release(settings);
	}
}

void AddImageSourceDlg::onOpacityValChanged(int val)
{
	string name = obs_source_get_display_name("chroma_key_filter");
	if (name.empty())
		return;

	if (m_pSource) {
		OBSSource filter = obs_source_get_filter_by_name(m_pSource, name.c_str());
		if (!filter)
			return;

		obs_data_t *settings = obs_source_get_settings(filter);
		obs_data_set_int(settings, "opacity", val);
		obs_source_update(filter, settings);
		obs_data_release(settings);
	}

	ui.label_opacity_val->setNum(val);
}

void AddImageSourceDlg::onDialogClosed()
{
	if (m_bNeedInit)
		main->CancelCurrentSource(getSourceName());

	close();
}

void AddImageSourceDlg::InitUI()
{
	ui.button_close->setCursor(Qt::PointingHandCursor);
	ui.button_cancel->setCursor(Qt::PointingHandCursor);
	ui.button_confirm->setCursor(Qt::PointingHandCursor);

	ui.slider_opacity->setMinimum(0);
	ui.slider_opacity->setMaximum(100);
	ui.slider_opacity->setSingleStep(1);
	ui.slider_opacity->setValue(100);
}

void AddImageSourceDlg::InitConnect()
{
	connect(ui.button_close, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
	connect(m_pSelectedButton, SIGNAL(onSigClicked()), this, SLOT(onOpenImage()));
	connect(ui.slider_opacity, SIGNAL(valueChanged(int)), this, SLOT(onOpacityValChanged(int)));
}

void AddImageSourceDlg::InitSource()
{
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	if (!m_pSource) {
		int index = 1;
		while (true)
		{
			QString name = QStringLiteral("图片%1").arg(index);
			obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
			if (source)
			{
				obs_source_release(source);
				index++;
			}
			else
			{
				source = obs_source_create("image_source", name.toStdString().c_str(), NULL, nullptr);
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
		if (strcmp(obs_property_name(property), "file") == 0) {
			file_property = property;
		}
		obs_property_next(&property);
	}

	AddNewFilter("chroma_key_filter");

	auto addDrawCallback = [this]()
	{
		obs_display_add_draw_callback(ui.wgt_preview->GetDisplay(),
			AddImageSourceDlg::DrawPreview, this);
	};	connect(ui.wgt_preview, &OBSQTDisplay::DisplayCreated,
		addDrawCallback);

	if (!m_ImgPath.isEmpty())
	{
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		if (!settings)
			return;

		obs_data_set_string(settings, "file", QT_TO_UTF8(m_ImgPath));
		obs_source_update(m_pSource, settings);
		obs_data_release(settings);
	}
}

void AddImageSourceDlg::AddNewFilter(const char* id)
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
				obs_data_set_int(settings, "similarity", 1);
				obs_source_update(filter, settings);
				obs_data_release(settings);
				obs_source_release(filter);
			}
		}
	}
}

void AddImageSourceDlg::DrawPreview(void *data, uint32_t cx, uint32_t cy)
{
	AddImageSourceDlg *window = static_cast<AddImageSourceDlg*>(data);

	if (!window->m_pSource)
		return;

	uint32_t sourceCX = max(obs_source_get_width(window->m_pSource), 1u);
	uint32_t sourceCY = max(obs_source_get_height(window->m_pSource), 1u);

	int   x, y;
	int   newCX, newCY;
	float scale;

	GetScaleAndCenterPos(sourceCX, sourceCY, cx, cy, x, y, scale);

	newCX = int(scale * float(sourceCX));
	newCY = int(scale * float(sourceCY));

	gs_viewport_push();
	gs_projection_push();
	gs_ortho(0.0f, float(sourceCX), 0.0f, float(sourceCY), -100.0f, 100.0f);
	gs_set_viewport(x, y, newCX, newCY);

	obs_source_video_render(window->m_pSource);

	gs_projection_pop();
	gs_viewport_pop();
}
