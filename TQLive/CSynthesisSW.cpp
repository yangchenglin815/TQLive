#include "CSynthesisSW.h"
#include "CLiveContentSelectionDlg.h"
#include "CController.h"
#include "CCompetitionButton.h"
#include "CGeneralData.h"


CSynthesisSW::CSynthesisSW(QWidget *parent)
	: QWidget(parent)
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

	this->setStyleSheet(qsStyleSheet);

	connect(CTaskNotification::GetInstance(), &CTaskNotification::signalAcquireCompetitionList, this, &CSynthesisSW::slotAcquireCompetitionList);

	ui.scrollArea->setStyleSheet("QScrollBar {width: 4px; padding-top: 0px}"
		"QScrollBar::handle {width: 4px; border-radius: 2px; margin: 0px; border-width: 0px;}");
	ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CSynthesisSW::~CSynthesisSW()
{
}

void CSynthesisSW::slotModuleSwitch(int nModuleType)
{
	if (nModuleType != SYSTHESIS_MODULE)
	{
		return;
	}

	if (m_bNeedLoad == false)
	{
		m_bNeedLoad = true;
		CController::GetInstance().AcquireCompetitionList(SYNTHESIS_TYPE, this);
	}
}

void CSynthesisSW::slotAcquireCompetitionList(int nStateCode, const QString &qsMsg, QList<ST_COMPETITION_INFO> obLCompetitionList, const void *userData)
{
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

			CCompetitionButton *button = new CCompetitionButton(this);
			button->setMinimumSize(QSize(93, 26));
			button->setMaximumSize(QSize(93, 26));
			button->setText(obLCompetitionList[i].qsCompetitionName);
			button->SetUserData(obLCompetitionList[i].nCompetitionID);

			m_obMType.insert(obLCompetitionList[i].nCompetitionID, obLCompetitionList[i].qsCompetitionName);
			ui.gridLayout->addWidget(button, nRowSeq, nColumnSeq);
			connect(button, SIGNAL(clicked()), this, SLOT(slotMatchesSelection()));
		}
	}
}

void CSynthesisSW::slotMatchesSelection()
{
	CCompetitionButton *button = (CCompetitionButton *)this->sender();
	int nCompetitionID = button->GetUserID();

	ST_MATCH_INFO stMatchInfo;
	stMatchInfo.qsCompetitionName = m_obMType[nCompetitionID];
	stMatchInfo.qsCompetitionName = QString::fromLocal8Bit("×ÛºÏ-") + stMatchInfo.qsCompetitionName;
	stMatchInfo.nCategoryID = SYNTHESIS_TYPE;
	stMatchInfo.nMatchID = nCompetitionID;

	emit signalItemSelection(stMatchInfo);
}

