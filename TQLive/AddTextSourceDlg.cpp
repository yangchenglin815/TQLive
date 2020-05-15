/***********************************************************************************
*								AddTextSourceDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-12
*
* Description: 添加文本资源对话框
***********************************************************************************/
#include "AddTextSourceDlg.h"
#include "FramelessHelper.h"
#include "window-basic-main.hpp"
#include "display-helpers.hpp"
#include "qt-wrappers.hpp"
#include "obs-app.hpp"
#include "SourceIniJson.h"
#include <QDir>

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

AddTextSourceDlg::AddTextSourceDlg(QWidget *parent, OBSSource source, bool initFlag)
	: QDialog(parent)
	, m_pSource(source)
	, m_bNeedInit(initFlag)
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

AddTextSourceDlg::~AddTextSourceDlg()
{

}

void AddTextSourceDlg::Load()
{
	if (m_pSource) {
		QString name = QT_UTF8(obs_source_get_name(m_pSource));
		for (auto &iter : SourceIniJson::instance()->m_pSourceIniList) {
			if (iter.sourceName == name) {
				ui.lineEdit_text->setText(iter.text);
				break;
			}
		}
	}

	show();
}

QString AddTextSourceDlg::getSourceName()
{
	QString sourceName;
	if (m_pSource) {
		sourceName = QT_UTF8(obs_source_get_name(m_pSource));
	}
	return sourceName;
}

void AddTextSourceDlg::Release()
{
	int index = -1;
	bool isOperate = false;
	for (auto &iter : SourceIniJson::instance()->m_pSourceIniList) {
		index++;
		if (iter.sourceName == getSourceName()) {
			isOperate = true;
			break;
		}
	}

	if (index > -1 && isOperate) {
		SourceIniJson::instance()->m_pSourceIniList.removeAt(index);
		SourceIniJson::instance()->Write();
	}
}

void AddTextSourceDlg::InitUI()
{
	ui.button_close->setCursor(Qt::PointingHandCursor);
	ui.button_cancel->setCursor(Qt::PointingHandCursor);
	ui.button_confirm->setCursor(Qt::PointingHandCursor);

	ui.lineEdit_text->setMaxLength(TEXT_LENGTH_MAX);
}

void AddTextSourceDlg::InitConnect()
{
	connect(ui.button_close, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(onDialogClosed()));
	connect(ui.button_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
	connect(ui.lineEdit_text, SIGNAL(textChanged(const QString &)), this, SLOT(onTextChanged(const QString &)));
}

void AddTextSourceDlg::InitSource()
{
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
		return;

	if (!m_pSource) {
		int index = 1;
		while (true)
		{
			QString name = QStringLiteral("文本%1").arg(index);
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
			text_property = property;
		}
		obs_property_next(&property);
	}

	onTextChanged("");
}

void AddTextSourceDlg::onDialogClosed()
{
	if (m_bNeedInit) {
		main->CancelCurrentSource(getSourceName());
	}

	close();
}

void AddTextSourceDlg::onConfirmClicked()
{
	if (m_pSource) {
		obs_data_t *settings = obs_source_get_settings(m_pSource);
		if (!settings)
			return;

		obs_data_set_string(settings, "file", QT_TO_UTF8(m_textSourceIniData.path));
		obs_source_update(m_pSource, settings);
		obs_data_release(settings);

		int index = -1;
		for (auto &iter : SourceIniJson::instance()->m_pSourceIniList) {
			if (iter.sourceName == m_textSourceIniData.sourceName) {
				iter.text = m_textSourceIniData.text;
				iter.path = m_textSourceIniData.path;
				index++;
			}
		}

		if (index == -1) {
			SourceIniJson::instance()->m_pSourceIniList.append(m_textSourceIniData);
		}

		SourceIniJson::instance()->Write();
	}

	m_bNeedInit = false;
	main->SaveProject();

	close();
}

void AddTextSourceDlg::onTextChanged(const QString &text)
{
	QString name = QT_UTF8(obs_source_get_name(m_pSource));
	QString path = QStringLiteral("./image/%1.png").arg(name);
	ui.wgt_template->RenderText(text, path);

	m_textSourceIniData.sourceName = name;
	m_textSourceIniData.text = text;
	m_textSourceIniData.path = path;
}
