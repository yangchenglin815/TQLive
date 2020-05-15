#include "CLiveScheduleWidget.h"
#include "CLiveContentSelectionDlg.h"
#include "CTableViewImpl.h"
#include <QTableView>
#include "CController.h"

CLiveScheduleWidget::CLiveScheduleWidget(QWidget *parent)
	: QWidget(parent)
	, m_bNeedLoad(false)
{
	ui.setupUi(this);
	//this->setStyleSheet("background-color: rgba(255, 0, 255, 255);"
	//	"border:0px solid rgba(88, 88, 88, 1);");

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
	//	//"QTableView::item {"
	//	//"border-width: 0px 10px 10px 0px; "							// QTableView±ß¿ò
	//	//"border-color: rgb(0, 0, 255);"
	//	//"border-style: solid;"
	//	//"}"
	//);

	//connect(ui.tv_live_schedule_data, SIGNAL(signalEditorBtnClicked(int)), this, SLOT(slotEditorBtnClicked(int)));

	//m_pTableView->ResetData(obLDataItemList);

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
		//"QTableView::item {"
		//"border-width: 0px 10px 10px 0px; "							// QTableView±ß¿ò
		//"border-color: rgb(0, 0, 255);"
		//"border-style: solid;"
		//"}"
	);

	//connect(m_pTableView, SIGNAL(signalConfirmChoice(int)), this, SLOT(slotEditorBtnClicked(int)));

	connect(m_pTableView, &CTableViewImpl::signalItemSelection, this, &CLiveScheduleWidget::slotItemSelection);
	connect(CTaskNotification::GetInstance(), &CTaskNotification::signalAcquireLiveScheduleList, this, &CLiveScheduleWidget::slotAcquireLiveScheduleList);

}

CLiveScheduleWidget::~CLiveScheduleWidget()
{
}

void CLiveScheduleWidget::slotAcquireLiveScheduleList(int nStatusCode, const QString &qsMsg, QList<ST_MATCH_INFO> obLMatchList)
{
	if (nStatusCode == 200)
	{
		m_pTableView->ResetData(obLMatchList);

		emit sigOnGroupNumberChanged();
	}
}

void CLiveScheduleWidget::slotModuleSwitch(int nModuleType)
{
	if (nModuleType != m_nModuleID)
	{
		return;
	}

	if (m_bNeedLoad == false)
	{
		m_bNeedLoad = true;
		CController::GetInstance().AcquireLiveScheduleList();
	}
}

void CLiveScheduleWidget::slotEditorBtnClicked(int index)
{
	emit signalEditorBtnClicked(index);
}

void CLiveScheduleWidget::slotItemSelection(ST_MATCH_INFO &stMatchInfo)
{
	QString qsCategoryName;

	switch (stMatchInfo.nCategoryID)
	{
	case 1:
	{
		qsCategoryName = QString::fromLocal8Bit("ÀºÇò-");
		break;
	}
	case 2:
	{
		qsCategoryName = QString::fromLocal8Bit("×ãÇò-");
		break;
	}
	case 3:
	{
		qsCategoryName = QString::fromLocal8Bit("×ÛºÏ-");
		break;
	}
	default:
	{
		qsCategoryName = QString::fromLocal8Bit("Î´¶¨Òå-");
		break;
	}

	}
	
	//stMatchInfo.qsCompetitionName = QString::fromLocal8Bit("Î´¶¨Òå-") + stMatchInfo.qsCompetitionName;
	stMatchInfo.qsCompetitionName = qsCategoryName + stMatchInfo.qsCompetitionName;
	emit signalItemSelection(stMatchInfo);
}

