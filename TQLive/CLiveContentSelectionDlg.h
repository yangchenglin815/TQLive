#pragma once

#include <QDialog>
#include "ui_CLiveContentSelectionDlg.h"
#include <QStackedWidget>
#include "CTaskNotification.h"
#include "CDataCentre.h"


class CCustomButtonGroup;

enum EN_MODULE_TYPE
{
	LIVE_SCHEDULE_MODULE,
	FOOTBALL_MODULE,
	BASKETBALL_MODULE,
	SYSTHESIS_MODULE
};

class CLiveContentSelectionDlg : public QDialog
{
	Q_OBJECT

public:
	CLiveContentSelectionDlg(int closeFlag, QWidget *parent = Q_NULLPTR);
	~CLiveContentSelectionDlg();
	//void SwitchTabModule(EN_MODULE_TYPE eModuleType);
	void SwitchTabModule(int nModuleID);

public slots:
	void slotTabButtonSwitch(int index);
	void slotEditorBtnClicked(int index);
	void slotItemSelection(ST_MATCH_INFO &stMatchInfo);
	void slotAcquireLiveCategory(int nStatusCode, const QString &qsMsg, const QList<ST_LIVE_CATEGORY_INFO> &obList, const void *userData);
	void slotOnGroupNumberChanged();

signals:
	void signalLiveContentUpdate(ST_MATCH_INFO &stMatchInfo);
	void signalModuleSwitch(int);
	//void signalLiveContentUpdate(const QString &qsCategoryName, const QString &qsContentName);
	
protected:
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::CLiveContentSelectionDlg ui;
	CCustomButtonGroup *m_pCustomButtonGroup;
	QStackedWidget m_swModuleContainer;
};
