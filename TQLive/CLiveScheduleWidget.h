#pragma once

#include <QWidget>
#include "ui_CLiveScheduleWidget.h"
#include "CDataCentre.h"

class CTableViewImpl;

class CLiveScheduleWidget : public QWidget
{
	Q_OBJECT

public:
	CLiveScheduleWidget(QWidget *parent = Q_NULLPTR);
	~CLiveScheduleWidget();

	void SetModuleID(int nModuleID)
	{
		m_nModuleID = nModuleID;
	}

public slots:
	void slotModuleSwitch(int nModuleType);
	void slotEditorBtnClicked(int index);
	void slotAcquireLiveScheduleList(int, const QString &, QList<ST_MATCH_INFO> obLCompetitionList);
	void slotItemSelection(ST_MATCH_INFO &stMatchInfo);

signals:
	void signalEditorBtnClicked(int index);
	void signalItemSelection(ST_MATCH_INFO &stMatchInfo);
	void sigOnGroupNumberChanged();

private:
	Ui::CLiveScheduleWidget ui;
	CTableViewImpl *m_pTableView;
	bool m_bNeedLoad;
	int m_nModuleID;
	
};
