#pragma once

#include <QStackedWidget>
#include "ui_CUniversalSW.h"
#include "CDataCentre.h"

class CTableViewImpl;



class CUniversalSW : public QStackedWidget
{
	Q_OBJECT

public:
	CUniversalSW(QWidget *parent = Q_NULLPTR);
	~CUniversalSW();

	enum EN_PAGE_TYPE
	{
		kCategoryPage,
		kMatchesPage
	};

	void SetModuleID(int nModuleID)
	{
		m_nModuleID = nModuleID;
	}

	void SetCategoryID(int nCategoryID)
	{
		m_nCategoryID = nCategoryID;
	}

	void SetCategoryName(const QString &qsCategoryName)
	{
		m_qsCategoryName = qsCategoryName;
	}

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
	Ui::CUniversalSW ui;
	CTableViewImpl *m_pTableView;
	bool m_bNeedLoad;
	int m_nModuleID;
	int m_nCategoryID;
	QString m_qsCategoryName;
};