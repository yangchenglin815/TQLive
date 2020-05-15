#include "CWaterMark.h"
#include "window-basic-main.hpp"
#include "window-basic-source-select.hpp"
#include "qt-wrappers.hpp"
#include "obs-app.hpp"
#include <QPainter>
#include "CDataCentre.h"

struct AddSourceData {
	obs_source_t *source;
	bool visible;
	obs_sceneitem_t **pSceneItem;
};

static void AddSource(void *_data, obs_scene_t *scene)
{
	AddSourceData *data = (AddSourceData *)_data;

	*data->pSceneItem = obs_scene_add(scene, data->source);

	// 设置水印出现的位置
	vec2 pos;
	vec2_set(&pos, 1620.00f, 40.00f);
	obs_sceneitem_set_pos(*data->pSceneItem, &pos);

	// 设置水印缩放大小
	//vec2 size;
	//vec2_set(&size, 2.00f, 2.00f);
	//obs_sceneitem_set_scale(*data->pSceneItem, &size);
	obs_sceneitem_set_visible(*data->pSceneItem, data->visible);

}

CWaterMark::CWaterMark(QObject *parent)
	: QObject(parent)
{
}

CWaterMark::~CWaterMark()
{
	//obs_source_release(m_pSource);
}

obs_source_t *CWaterMark::Add(const char *path, const char * id)
{
	OBSBasic *main = reinterpret_cast<OBSBasic *>(App()->GetMainWindow());
	OBSScene scene = main->GetCurrentScene();
	if (!scene)
	{
		return NULL;
	}

	int index = 1;

	QString qsName("logo_water_mark");
	obs_source_t *source = obs_get_source_by_name(qsName.toStdString().c_str());
	//if (source)																		// 重名处理
	//{
	//	obs_source_release(source);
	//	return source;
	//}

	source = obs_source_create(id, qsName.toStdString().c_str(), NULL, nullptr);

	AddSourceData data;
	data.source = source;
	data.visible = true;
	data.pSceneItem = &this->m_pSceneItem;

	obs_enter_graphics();
	obs_scene_atomic_update(scene, AddSource, &data);
	obs_leave_graphics();

	m_pSource = source;

	return source;

	//QString qsText("32147");
	//QImage image(QString::fromLocal8Bit(path), "PNG");
	//QPainter painter(&image);
	//painter.setPen(QColor(255, 0, 0, 255));
	//painter.drawText(5, 20, qsText);
	//QString qsDstPath("./images/destination_logo.png");
	//image.save(qsDstPath);

	//OBSData settings = obs_source_get_settings(m_pSource);
	////obs_data_set_string(settings, "file", QT_TO_UTF8(QString(path)));
	//obs_data_set_string(settings, "file", QT_TO_UTF8(qsDstPath));
	//obs_source_update(m_pSource, settings);
	//obs_data_release(settings);
}

void CWaterMark::Add(const char *path, const char * id, obs_scene_t *scene)
{
	if (!scene)
	{
		return;
	}

	QString name("logo_water_mark");
	obs_source_t *source = obs_get_source_by_name(name.toStdString().c_str());
	if (source)																		// 重名处理
	{
		obs_source_release(source);
		return;
	}

	source = obs_source_create(id, name.toStdString().c_str(), NULL, nullptr);

	AddSourceData data;
	data.source = source;
	data.visible = true;

	data.pSceneItem = &this->m_pSceneItem;

	obs_enter_graphics();
	obs_scene_atomic_update(scene, AddSource, &data);
	obs_leave_graphics();

	//m_pSource = source;

	//OBSData settings = obs_source_get_settings(m_pSource);
	//obs_data_set_string(settings, "file", QT_TO_UTF8(QString(path)));
	//obs_source_update(m_pSource, settings);
	//obs_data_release(settings);
}

void CWaterMark::BringLayerToFront()
{
	OBSBasic *main = reinterpret_cast<OBSBasic *>(App()->GetMainWindow());
	OBSScene scene = main->GetCurrentScene();
	obs_sceneitem_t *item = obs_scene_find_source(scene, "logo_water_mark");
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_TOP);
}

void CWaterMark::CompositeLayer()
{
	QString path("./images/logo_with_roomnumber.png");
	QString qsText = g_dataCentre.stUserInformation.qsRoomNumber;
	QImage image(path, "PNG");
	QPainter painter(&image);
	QString qsFamily = QString::fromLocal8Bit("微软雅黑");
	QFont font(qsFamily);
	font.setPixelSize(24);
	painter.setFont(font);

	painter.setPen(QColor(255, 255, 255, 127));
	painter.drawText(QRect(80, 74, 120, 28), qsText);
	QString qsDstPath("./images/destination_logo.png");
	bool result = image.save(qsDstPath);

	OBSData settings = obs_source_get_settings(m_pSource);
	//obs_data_set_string(settings, "file", QT_TO_UTF8(QString(path)));
	obs_data_set_string(settings, "file", QT_TO_UTF8(qsDstPath));
	obs_source_update(m_pSource, settings);
	obs_data_release(settings);
}

void CWaterMark::ResetBackgroudLayer(obs_source_t *source)
{
	if (source == NULL)
	{
		source = m_pSource;
	}

	QString qsDstPath("./images/logo.png");
	OBSData settings = obs_source_get_settings(source);
	//obs_data_set_string(settings, "file", QT_TO_UTF8(QString(path)));
	obs_data_set_string(settings, "file", QT_TO_UTF8(qsDstPath));
	obs_source_update(source, settings);
	obs_data_release(settings);
	m_pSource = source;

	//obs_sceneitem_t *waterMarkItem = obs_sceneitem_get_so
	//vec2 pos;
	//vec2_set(&pos, 1700.00f, 0.00f);
	//obs_sceneitem_set_pos(*data->pSceneItem, &pos);
}

void CWaterMark::ShowWithNoRoomNO(obs_source_t *source)
{
	QString qsDstPath("./images/logo.png");
	OBSData settings = obs_source_get_settings(source);
	
	obs_data_set_string(settings, "file", QT_TO_UTF8(qsDstPath));
	obs_source_update(source, settings);
	obs_data_release(settings);
}

void CWaterMark::ShowWithRoomNO(obs_source_t *source)
{
	QString path("./images/logo_with_roomnumber.png");
	QString qsText = QString::fromLocal8Bit("房间号：") + g_dataCentre.stUserInformation.qsRoomNumber;
	
	QImage image(path, "PNG");
	QString qsFamily = QString::fromLocal8Bit("微软雅黑");
	QFont font(qsFamily);
	//font.setStyleStrategy(QFont::NoAntialias);
	font.setPixelSize(28);
	font.setWeight(QFont::Bold);

	QPainter painter(&image);
	painter.setFont(font);
	painter.setPen(QColor(255, 255, 255, 127));
	//painter.setRenderHint(QPainter::TextAntialiasing);
	painter.drawText(QRect(6, 82, 250, 38), qsText);

	QString qsDstPath("./images/destination_logo.png");
	bool result = image.save(qsDstPath);

	OBSData settings = obs_source_get_settings(source);
	obs_data_set_string(settings, "file", QT_TO_UTF8(qsDstPath));
	obs_source_update(source, settings);
	obs_data_release(settings);
}



