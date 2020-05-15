#include "CFootballSW.h"
#include "CLiveContentSelectionDlg.h"
#include <qpushbutton.h>
#include "CTableViewImpl.h"
#include <QTableView>
#include "CController.h"
#include "CCompetitionButton.h"
#include "CGeneralData.h"

CFootballSW::CFootballSW(QWidget *parent)
	: QStackedWidget(parent)
	, m_bNeedLoad(false)
{
	ui.setupUi(this);

	QString qsStyleSheet = "QPushButton {"
		"font-size:12px;"
		//"font-family:Source Han Sans CN;"
		"font-weight:400;"
		"padding:0px;"
		"color:rgba(153, 153, 153, 1);"
		"border-radius:2px;"
		"background-color: rgb(47, 47, 47, 255);"		// ×¢Òâ	
		"}"
		"QPushButton:hover {"
		"color:rgba(74,177,52,1);"
		"}"
		"QPushButton:pressed {"
		"color:rgba(74,177,52,1);"
		"}";

	ui.wd_football_category_page->setStyleSheet(qsStyleSheet);

	//QList<QString> obLCompetition;
	//obLCompetition.push_back(QString::fromLocal8Bit("Ó¢³¬"));
	//obLCompetition.push_back(QString::fromLocal8Bit("Î÷¼×"));
	//obLCompetition.push_back(QString::fromLocal8Bit("µÂ¼×"));
	//obLCompetition.push_back(QString::fromLocal8Bit("Òâ¼×"));
	//obLCompetition.push_back(QString::fromLocal8Bit("·¨¼×"));
	//obLCompetition.push_back(QString::fromLocal8Bit("ÆÏ³¬"));
	//obLCompetition.push_back(QString::fromLocal8Bit("Å·¹Ú"));
	//obLCompetition.push_back(QString::fromLocal8Bit("Å·Áª±­"));
	//obLCompetition.push_back(QString::fromLocal8Bit("¹úÍõ±­"));

	//int nRowSeq, nColumnSeq;

	//for (int i = 0; i < obLCompetition.size(); i++)
	//{
	//	nRowSeq = i / 6;
	//	nColumnSeq = i % 6;

	//	QPushButton *button = new QPushButton(ui.wd_football_category_page);
	//	button->setMinimumSize(QSize(93, 26));
	//	button->setMaximumSize(QSize(93, 26));
	//	button->setText(obLCompetition[i]);
	//	ui.gridLayout->addWidget(button, nRowSeq, nColumnSeq);
	//	connect(button, SIGNAL(clicked()), this, SLOT(slotMatchesSelection()));
	//}

	m_pTableView = new CTableViewImpl(this);
	QTableView *pTableView = m_pTableView->currentTableView();
	ui.verticalLayout_A->addWidget(pTableView);

	//ST_COMPETITION_INFO stCompetitionInfo;
	//stCompetitionInfo.qsDateTime = "2019/8/30 21:00";
	//stCompetitionInfo.qsType = QString::fromLocal8Bit("Ó¢³¬");
	//stCompetitionInfo.qsContent = QString::fromLocal8Bit("Âü³Ç vs Ë¹ÍÐ¿Ë³Ç");

	//QList<ST_COMPETITION_INFO> obLDataItemList;
	//obLDataItemList.push_back(stCompetitionInfo);
	//obLDataItemList.push_back(stCompetitionInfo);
	//obLDataItemList.push_back(stCompetitionInfo);
	//obLDataItemList.push_back(stCompetitionInfo);

	//ui.tv_live_schedule_data->ResetData(obLDataItemList);

	//ui.tv_live_schedule_data->setStyleSheet("QTableView {"
	//	"border-width: 0px 0px 0px 0px; "							// QTableView±ß¿ò
	//	"border-color: rgb(0, 0, 255);"
	//	"border-style: solid;"
	//	"gridline-color:rgb(47, 47, 47, 1);"
	//	"font-size:12px;"
	//	//"font-family:Source Han Sans CN;"
	//	"font-weight:400;"
	//	"color:rgba(153, 153, 153, 1);"
	//	"}"
	//);

	pTableView->setStyleSheet("QTableView {"
			"border-width: 0px 0px 0px 0px; "							// QTableView±ß¿ò
			"border-color: rgb(0, 0, 255);"
			"border-style: solid;"
			"gridline-color:rgb(47, 47, 47, 1);"
			"font-size:12px;"
			//"font-family:Source Han Sans CN;"
			"font-weight:400;"
			"color:rgba(153, 153, 153, 1);"
			"}"
		);

	//m_pTableView->ResetData(obLDataItemList);

	connect(ui.pb_go_back, SIGNAL(clicked()), this, SLOT(slotGoBackCompetitionPage()));
	//connect(m_pTableView, &CTableViewImpl::signalConfirmChoice, this, &CFootballSW::slotConfirmChoice);
	connect(m_pTableView, &CTableViewImpl::signalItemSelection, this, &CFootballSW::slotItemSelection);
	connect(CTaskNotification::GetInstance(), &CTaskNotification::signalAcquireCompetitionList, this, &CFootballSW::slotAcquireCompetitionList);
	connect(CTaskNotification::GetInstance(), &CTaskNotification::signalAcquireMatchList, this, &CFootballSW::slotAcquireMatchList);

	ui.scrollArea->setStyleSheet("QScrollBar {width: 4px; padding-top: 0px}"
		"QScrollBar::handle {width: 4px; border-radius: 2px; margin: 0px; border-width: 0px;}");
	ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CFootballSW::~CFootballSW()
{
}

void CFootballSW::slotModuleSwitch(int nModuleType)
{
	if (nModuleType != FOOTBALL_MODULE)
	{
		return;
	}

	if (m_bNeedLoad == false)
	{
		m_bNeedLoad = true;
		CController::GetInstance().AcquireCompetitionList(FOOTBALL_TYPE, this);
	}
}

void CFootballSW::slotAcquireCompetitionList(int nStateCode, const QString &qsMsg, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData)
{
	//if (nStateCode == 200)
	//{
	//	int nRowSeq, nColumnSeq;

	//	for (int i = 0; i < g_dataCentre.m_obLCompetitionList.size(); i++)
	//	{
	//		nRowSeq = i / 6;
	//		nColumnSeq = i % 6;

	//		QPushButton *button = new QPushButton(ui.wd_football_category_page);
	//		button->setMinimumSize(QSize(93, 26));
	//		button->setMaximumSize(QSize(93, 26));
	//		button->setText(g_dataCentre.m_obLCompetitionList[i]);
	//		ui.gridLayout->addWidget(button, nRowSeq, nColumnSeq);
	//		connect(button, SIGNAL(clicked()), this, SLOT(slotMatchesSelection()));
	//	}
	//}

	if (userData != this)
	{
		return;
	}

	if (nStateCode == 200)
	{
		int nRowSeq, nColumnSeq;

		for (int i = 0; i < obLCompetitionList.size(); i++)
		{
			nRowSeq = i / 6;
			nColumnSeq = i % 6;

			CCompetitionButton *button = new CCompetitionButton(ui.wd_football_category_page);
			button->setMinimumSize(QSize(93, 26));
			button->setMaximumSize(QSize(93, 26));
			button->setText(obLCompetitionList[i].qsCompetitionName);
			button->SetUserData(obLCompetitionList[i].nCompetitionID);
			ui.gridLayout->addWidget(button, nRowSeq, nColumnSeq);
			connect(button, SIGNAL(clicked()), this, SLOT(slotMatchesSelection()));
		}
	}
}

void CFootballSW::slotMatchesSelection()
{
	CCompetitionButton *button = (CCompetitionButton *)this->sender();
	int nCompetitionID = button->GetUserID();

	CController::GetInstance().AcquireMatchList(FOOTBALL_TYPE, nCompetitionID, this);

	this->setCurrentIndex(kMatchesPage);
}

void CFootballSW::slotGoBackCompetitionPage()
{
	this->setCurrentIndex(kCategoryPage);
}

void CFootballSW::slotAcquireMatchList(int nStateCode, const QString &qsMsg, QList<ST_MATCH_INFO> obLMatchList, const void *userData)
{
	if (userData != this)
	{
		return;
	}

	if (nStateCode == 200)
	{
		m_pTableView->ResetData(obLMatchList);
	}

}

void CFootballSW::slotConfirmChoice(int nMatchID)
{

}

void CFootballSW::slotItemSelection(ST_MATCH_INFO &stMatchInfo)
{
	stMatchInfo.qsCompetitionName = QString::fromLocal8Bit("×ãÇò-") + stMatchInfo.qsCompetitionName;
	emit signalItemSelection(stMatchInfo);
}