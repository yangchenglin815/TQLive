#pragma once

#include <QDialog>
#include <obs.hpp>
#include <memory>
#include "StructData.h"
#include "ui_AddMediaSourceDlg.h"

enum MediaAddPageType {
	PAGE_MEDIA_SOURCE_ADD = 0,
	PAGE_MEDIA_SOURCE_SET = 1
};

enum ItemStateType {
	STATE_COMMON = 0,
	STATE_ACTIVE,
	STATE_HOVER,
	STATE_SELECTED
};

class OBSBasic;
class PlayMediaLitItem;
class CustomArrowWgt;
class AddMediaSourceDlg : public QDialog
{
	Q_OBJECT

	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

public:
	AddMediaSourceDlg(QWidget *parent, OBSSource source, bool initFlag);
	~AddMediaSourceDlg();

public:
	QString getSourceName();
	void Release();
	void Load();

private slots:
    void onCurrentPageChanged(int index);
	void onAddMediaClicked();
	void onLoopClicked();
	void onLeftButtonClicked();
	void onActiveItem(QListWidgetItem *current);
	void onPlaySliderPressed();
	void onPlaySliderReleased();
	void onDeleteItem(int id);
	void onUpItem(int id);
	void onDownItem(int id);
	void onPlayLastMedia();
	void onPlayNextMedia();
	void onSoundsValueChanged(int val);
	void onSoundsButtonClicked();
	void onLoopTypeChanged(int loopType);
	void onDialogClosed();

public slots:
	void onMediaPlayFinished();

signals:
	void onSigMediaSoundsValChanged(int val, QString sourceName);

protected:
	void timerEvent(QTimerEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);

private:
	void InitUI();
	void InitConnect();
	void InitSource();
	void AddItem(const MediaSourceData &mediaData);
	void UpdateList(QList<MediaSourceData> list);

	void LoopOnRandomPlay();
	void LoopOnSingleCycle();
	void LoopOnListCycle();

private:
	Ui::AddMediaSourceDlg ui;
	int m_nPageType = PAGE_MEDIA_SOURCE_ADD;
	int m_nTotalMs = 0;
	int m_nCurrPts = 0;
	int m_nCurrPlayIndex = -1;
	int m_nCurr_SoundsVal = 100;
	int m_nCurr_LoopType = LOOP_CYCLE;
	bool m_bNeedInit;

	CustomArrowWgt *m_pLoopMenu;
	QList<PlayMediaLitItem*> m_pPlayMediaItemList;

	OBSBasic *main;
	OBSSource m_pSource = nullptr;
	properties_t properties = nullptr;

	OBSSignal TotalTimeDurationSignal;
	OBSSignal CurrTimeDurationSignal;
	OBSSignal MediaPlayFinishedSignal;
};
