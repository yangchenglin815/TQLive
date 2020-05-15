#include "CBasketballSW.h"
#include "CLiveContentSelectionDlg.h"
#include "CController.h"
#include "CTaskNotification.h"
#include "CTableViewImpl.h"
#include <QTableView>
#include "CCompetitionButton.h"
#include "CGeneralData.h"

CBasketballSW::CBasketballSW(QWidget *parent)
	: QStackedWidget(parent)
	, m_bNeedLoad(false)
{
	ui.setupUi(this);

	QString qsStyleSheet = "QPushButton {"
		"font-size:12px;"
		//"font-family:Source Han Sans CN;"
		"font-weight:400;"
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

	ui.wd_basketball_category_page->setStyleSheet(qsStyleSheet);

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

	m_pTableView = new CTableViewImpl(this);

	ui.verticalLayout_A->addWidget(m_pTableView->currentTableView());


	connect(ui.pb_go_back, SIGNAL(clicked()), this, SLOT(slotGoBackCompetitionPage()));
	connect(m_pTableView, &CTableViewImpl::signalItemSelection, this, &CBasketballSW::slotItemSelection);
	//connect((QObject *)CTaskNotification::GetInstance(), SIGNAL(signalAcquireCompetitionList(int, const QString &)), this, SLOT(slotAcquireCompetitionList(int, const QString &)));
	connect(CTaskNotification::GetInstance(), &CTaskNotification::signalAcquireCompetitionList, this, &CBasketballSW::slotAcquireCompetitionList);
}

CBasketballSW::~CBasketballSW()
{
}

void CBasketballSW::slotModuleSwitch(int nModuleType)
{
	if (nModuleType != BASKETBALL_MODULE)
	{
		return;
	}

	if (m_bNeedLoad == false)
	{
		m_bNeedLoad = true;
		CController::GetInstance().AcquireCompetitionList(BASKETBALL_TYPE, this);
	}

	
}

void CBasketballSW::slotAcquireCompetitionList(int nStateCode, const QString &qsMsg, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData)
{
	if (userData != this)
	{
		return;
	}

	if (nStateCode == 200)
	{
		int nRowSeq, nColumnSeq;

		//for (int i = 0; i < g_dataCentre.m_obLCompetitionList.size(); i++)
		//{
		//	nRowSeq = i / 6;
		//	nColumnSeq = i % 6;

		//	QPushButton *button = new QPushButton(ui.wd_basketball_category_page);
		//	button->setMinimumSize(QSize(93, 26));
		//	button->setMaximumSize(QSize(93, 26));
		//	button->setText(g_dataCentre.m_obLCompetitionList[i]);
		//	ui.gridLayout->addWidget(button, nRowSeq, nColumnSeq);
		//	connect(button, SIGNAL(clicked()), this, SLOT(slotMatchesSelection()));
		//}

		for (int i = 0; i < obLCompetitionList.size(); i++)
		{
			nRowSeq = i / 6;
			nColumnSeq = i % 6;

			CCompetitionButton *button = new CCompetitionButton(ui.wd_basketball_category_page);
			button->setMinimumSize(QSize(93, 26));
			button->setMaximumSize(QSize(93, 26));
			button->setText(obLCompetitionList[i].qsCompetitionName);
			button->SetUserData(obLCompetitionList[i].nCompetitionID);
			ui.gridLayout->addWidget(button, nRowSeq, nColumnSeq);
			connect(button, SIGNAL(clicked()), this, SLOT(slotMatchesSelection()));
		}

		//ui.wd_basketball_category_page->update();
	}
}

void CBasketballSW::slotMatchesSelection()
{
	CCompetitionButton *button = (CCompetitionButton *)this->sender();
	int nCompetitionID = button->GetUserID();

	CController::GetInstance().AcquireMatchList(BASKETBALL_TYPE, nCompetitionID, this);

	this->setCurrentIndex(kMatchesPage);
}

void CBasketballSW::slotGoBackCompetitionPage()
{
	this->setCurrentIndex(kCategoryPage);
}

void CBasketballSW::slotItemSelection(ST_MATCH_INFO &stMatchInfo)
{
	//stMatchInfo.qsCompetitionName = QString::fromLocal8Bit("ÀºÇò-") + stMatchInfo.qsCompetitionName;
	stMatchInfo.qsCompetitionName = QString::fromLocal8Bit("ÀºÇò-") + stMatchInfo.qsCompetitionName;
	emit signalItemSelection(stMatchInfo);
}
