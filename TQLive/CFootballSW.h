#pragma once

#include <QStackedWidget>
#include "ui_CFootballSW.h"
#include "CDataCentre.h"

class CTableViewImpl;

enum EN_PAGE_TYPE
{
	kCategoryPage,
	kMatchesPage
};

class CFootballSW : public QStackedWidget
{
	Q_OBJECT

public:
	CFootballSW(QWidget *parent = Q_NULLPTR);
	~CFootballSW();

public slots:
	void slotModuleSwitch(int nModuleType);
	void slotMatchesSelection();
	void slotGoBackCompetitionPage();
	void slotConfirmChoice(int nMatchID);
	void slotItemSelection(ST_MATCH_INFO &stMatchInfo);
	void slotAcquireCompetitionList(int, const QString &, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData);
	void slotAcquireMatchList(int, const QString &, QList<ST_MATCH_INFO> obLMatchList, const void *userData);

signals:
	//void signalLiveContentUpdate(const ST_MATCH_INFO &);
	void signalItemSelection(ST_MATCH_INFO &stMatchInfo);

private:
	Ui::CFootballSW ui;
	CTableViewImpl *m_pTableView;
	bool m_bNeedLoad;
};
