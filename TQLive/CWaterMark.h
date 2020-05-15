#pragma once
#include <QObject>
#include <obs.hpp>
#include <memory>

class OBSBasic;
class CWaterMark : public QObject
{
	Q_OBJECT

	//using properties_delete_t = decltype(&obs_properties_destroy);
	//using properties_t = std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	CWaterMark(QObject *parent);
	~CWaterMark();
	obs_source_t *Add(const char *path, const char * id);
	void Add(const char *path, const char * id, obs_scene_t *scene);
	void BringLayerToFront();
	void CompositeLayer();
	void ResetBackgroudLayer(obs_source_t *source);
	void ShowWithNoRoomNO(obs_source_t *source);
	void ShowWithRoomNO(obs_source_t *source);

private:
	OBSSource m_pSource;

public:
	obs_sceneitem_t *m_pSceneItem;
};
