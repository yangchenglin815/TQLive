#pragma once

#include <QDialog>
#include <obs.hpp>
#include <memory>
#include "StructData.h"
#include "ui_AddCameraSourceDlg.h"

class AddCameraSourceDlg : public QDialog
{
	Q_OBJECT
	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	AddCameraSourceDlg(QWidget *parent, OBSSource source, bool initFlag);
	~AddCameraSourceDlg();

public:
	void Load();
	void Release();
	QString getSourceName();

signals:
	void onSigFlipHorizontal();

private slots:
	void onOpenCamera();
	void onPickColorRGB();
	void onRebackSettings();
	void onConfirmClicked();
	void onCancelClicked();

	void onThinBodyValueChanged(int value);
	void onWhitenPicValueChanged(int value);
	void onSimilarityValueChanged(int value);
	void onSmoothnessValueChanged(int value);
	void onSpillValueChanged(int value);

	void onCameraSelectedIndexChanged(int index);
	void onInputResolutionIndexChanged(int index);

	void onThinBodySwitchStateChanged(int state);
	void onWhitenPicSwitchStateChanged(int state);
	void onBgFilterSwitchStateChanged(int state);
	void onFlipHorizontalStateChanged(int state);
	void onFlipVerticalStateChanged(int state);

	void RemoveFilter(OBSSource filter);

private:
	void InitUI();
	void InitConnect();
	void InitSource();
	void InitBasicSettings();
	void InitSmartbeautification();
	void InitBackgroundFilter();
	void InitDirectionOfLens();
	void ReloadProperties();
	void ReloadCache();
	void ReloadIni();

	void LoadBasicSettings();
	void LoadSmartbeautification();
	void LoadBackgroundFilter();
	void LoadDirectionOfLens();

	void SaveBasicSettings();
	void SaveSmartbeautification();
	void SaveBackgroundFilter();
	void SaveDirectionOfLens();
	void SaveCameraCache();

	void AddCameraSelectedItem(obs_property_t *prop, size_t idx);
	void LoadCameraSelectedItems();
	void LoadOpenCameraButton();
	void LoadOuputResolution();
	void AddNewFilter(const char* id);
	void RemoveFilter();

	static void OBSSourceFilterRemoved(void *param, calldata_t *data);

private:
	Ui::AddCameraSourceDlg ui;
	bool m_bThinBodySwitch;
	bool m_bWhitenPicSwitch;
	bool m_bBgFilterSwitch;
	bool m_bIsActive = false;
	bool m_bNeedInit;

	//缓存未保存设置前的数据
	CameraCacheData m_cacheData;
// 	int m_nCameraItemCacheIdx = 0;
// 	int m_nResoultionCacheIdx = 0;
// 	bool m_bFilterOfBgCache = false;
// 	QColor m_cFilterOfColorCache = Qt::green;
// 	int m_nSimilarityValCache = 1;
// 	int m_nSmoothnessValCache = 1;
// 	int m_nSpillValCache = 1;
// 	int m_nFlipTypeCache = 0; //0:未翻转 1:水平 2:垂直 3:均翻转

	OBSSignal removeSignal;
	OBSSource m_pSource = nullptr;
	properties_t properties = nullptr;
	obs_property_t *dshow_property = nullptr;
	obs_property_t *active_property = nullptr;
	obs_property_t *output_property = nullptr;
	obs_property_t *flip_property = nullptr;
};
