#pragma once

#include <QStackedWidget>
#include "ui_CBasketballSW.h"
#include "CDataCentre.h"

class CTableViewImpl;

class CBasketballSW : public QStackedWidget
{
	Q_OBJECT

public:
	CBasketballSW(QWidget *parent = Q_NULLPTR);
	~CBasketballSW();

public slots:
	void slotModuleSwitch(int nModuleType);
	void slotMatchesSelection();
	void slotGoBackCompetitionPage();
	void slotAcquireCompetitionList(int, const QString &, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData);
	void slotItemSelection(ST_MATCH_INFO &stMatchInfo);

signals:
	void signalItemSelection(ST_MATCH_INFO &stMatchInfo);

private:
	enum EN_PAGE_TYPE
	{
		kCategoryPage,
		kMatchesPage
	};

	Ui::CBasketballSW ui;
	bool m_bNeedLoad;
	CTableViewImpl *m_pTableView;
};
