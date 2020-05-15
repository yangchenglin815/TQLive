#include "CLiveContentSelectionDlg.h"
#include "CCustomButtonGroup.h"
#include "CLiveScheduleWidget.h"
#include "CFootballSW.h"
#include "CBasketballSW.h"
#include "CSynthesisSW.h"
#include "CUniversalSW.h"
#include <QKeyEvent>
#include "CController.h"
#include "CDataCentre.h"
#include <stdio.h>
#include <math.h>

CLiveContentSelectionDlg::CLiveContentSelectionDlg(int closeFlag, QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	ui.setupUi(this);

	this->setStyleSheet("background-color: rgba(38, 38, 38, 1);"
		"border:1px solid rgba(88, 88, 88, 1);");

	//m_swModuleContainer.setStyleSheet("background-color: rgba(0, 0, 255, 255);"
	//	"border:none;");

	m_swModuleContainer.setStyleSheet("border:none;");

	// 样式表无效
	//ui.chb_head_bar->setStyleSheet("background-color:rgba(47,47,47,255);");
	//ui.chb_head_bar->SetBkColor(QColor(255, 0, 0, 255));

	//m_pCustomButtonGroup = new CCustomButtonGroup(this, HORIZONTAL_STYLE);
	//m_pCustomButtonGroup->resize(610, 46);
	//m_pCustomButtonGroup->SetCustomButtonGeometry(0, 46);

	//QString szBkImagePath[5];

	////szBkImagePath[0] = "live_schedule_normal.png"
	////szBkImagePath[0] = "live_schedule_normal.png";
	////szBkImagePath[0] = "live_schedule_normal.png";
	////szBkImagePath[0] = "live_schedule_normal.png";
	////szBkImagePath[0] = "live_schedule_normal.png";

	//QString szPrefix[4] = {":/TQLive/images/live_schedule_", ":/TQLive/images/football_", ":/TQLive/images/basketball_", ":/TQLive/images/synthesis_"};
	//QString szState[5] = { "normal.png", "over.png", "over.png", "over.png", "over.png"};

	//for (int i = 0; i < 5; i++)
	//{
	//	szBkImagePath[i] = szPrefix[0] + szState[i];
	//}

	//m_pCustomButtonGroup->AddTabButton("", szBkImagePath, 55, 14);

	//for (int i = 0; i < 5; i++)
	//{
	//	szBkImagePath[i] = szPrefix[1] + szState[i];
	//}
	//
	//m_pCustomButtonGroup->AddTabButton("", szBkImagePath, 28, 14);

	//for (int i = 0; i < 5; i++)
	//{
	//	szBkImagePath[i] = szPrefix[2] + szState[i];
	//}

	//m_pCustomButtonGroup->AddTabButton("", szBkImagePath, 28, 14);

	//for (int i = 0; i < 5; i++)
	//{
	//	szBkImagePath[i] = szPrefix[3] + szState[i];
	//}

	//m_pCustomButtonGroup->AddTabButton("", szBkImagePath, 28, 14);

	//m_pCustomButtonGroup->AddHorizontalSpacer();
	//ui.verticalLayout->insertWidget(1, m_pCustomButtonGroup);

	////m_pCustomButtonGroup->SetDefaultDownBtn(0);

	//// 注意Line加入时的对齐方式
	//QHBoxLayout *pHBoxLayout = new QHBoxLayout(NULL);
	//pHBoxLayout->setSpacing(0);
	//pHBoxLayout->setContentsMargins(0, 0, 0, 0);

	//QFrame *line = new QFrame(this);
	//line->setObjectName(QStringLiteral("line"));
	//line->setMinimumSize(QSize(610, 1));
	//line->setMaximumSize(QSize(610, 1));
	//line->setStyleSheet(QStringLiteral("border-color: rgba(47, 47, 47, 1);"));
	//line->setFrameShadow(QFrame::Plain);
	//line->setFrameShape(QFrame::HLine);

	//pHBoxLayout->addWidget(line);
	//ui.verticalLayout->insertLayout(2, pHBoxLayout);
	//
	////ui.verticalLayout->insertWidget(2, line);

	//connect(m_pCustomButtonGroup, SIGNAL(signalTabButtonSwitch(int)), SLOT(slotTabButtonSwitch(int)));

	//CLiveScheduleWidget *pLiveScheduleWidget = new CLiveScheduleWidget(&m_swModuleContainer);
	//connect(this, SIGNAL(signalModuleSwitch(int)), pLiveScheduleWidget, SLOT(slotModuleSwitch(int)));
	////connect(pLiveScheduleWidget, SIGNAL(signalEditorBtnClicked(int)), this, SLOT(slotEditorBtnClicked(int)));
	//connect(pLiveScheduleWidget, &CLiveScheduleWidget::signalItemSelection, this, &CLiveContentSelectionDlg::slotItemSelection);

	//CFootballSW *pFootballSW = new CFootballSW(&m_swModuleContainer);
	//connect(this, SIGNAL(signalModuleSwitch(int)), pFootballSW, SLOT(slotModuleSwitch(int)));
	//connect(pFootballSW, &CFootballSW::signalItemSelection, this, &CLiveContentSelectionDlg::slotItemSelection);

	//CBasketballSW *pBasketballSW = new CBasketballSW(&m_swModuleContainer);
	//connect(this, SIGNAL(signalModuleSwitch(int)), pBasketballSW, SLOT(slotModuleSwitch(int)));
	////connect(pBasketballSW, &CBasketballSW::signalItemSelection, this, &CLiveContentSelectionDlg::slotItemSelection);

	//CSynthesisSW *pSynthesisSW = new CSynthesisSW(&m_swModuleContainer);
	//connect(this, SIGNAL(signalModuleSwitch(int)), pSynthesisSW, SLOT(slotModuleSwitch(int)));
	//connect(pSynthesisSW, &CSynthesisSW::signalItemSelection, this, &CLiveContentSelectionDlg::slotItemSelection);

	connect(ui.button_close, &QPushButton::clicked, this, &QDialog::close);

	//m_swModuleContainer.addWidget(pLiveScheduleWidget);
	//m_swModuleContainer.addWidget(pFootballSW);
	//m_swModuleContainer.addWidget(pBasketballSW);
	//m_swModuleContainer.addWidget(pSynthesisSW);

	//ui.verticalLayout->insertWidget(3, &m_swModuleContainer);

	//m_pCustomButtonGroup->SetDefaultDownBtn(0);

	connect(CTaskNotification::GetInstance(), &CTaskNotification::signalAcquireLiveCategory, this, &CLiveContentSelectionDlg::slotAcquireLiveCategory);

	CController::GetInstance().AcquireLiveCategoryList();

	ui.button_close->setHidden(!closeFlag);
}

CLiveContentSelectionDlg::~CLiveContentSelectionDlg()
{
}

void CLiveContentSelectionDlg::slotAcquireLiveCategory(int nStatusCode, const QString &qsMsg, const QList<ST_LIVE_CATEGORY_INFO> &obList, const void *userData)
{
	m_pCustomButtonGroup = new CCustomButtonGroup(this, HORIZONTAL_STYLE);
	m_pCustomButtonGroup->resize(610, 46);
	m_pCustomButtonGroup->setMaximumWidth(610);
	m_pCustomButtonGroup->SetCustomButtonGeometry(0, 46);

	QString szBkImagePath[5];

	//QString qsPrefix = ":/TQLive/images/live_schedule_";
	//QString szState[5] = { "normal.png", "over.png", "over.png", "over.png", "over.png" };

	//for (int i = 0; i < 5; i++)
	//{
	//	szBkImagePath[i] = qsPrefix + szState[i];
	//}

	// 直播排期分类
	m_pCustomButtonGroup->AddTabButton(QString::fromLocal8Bit("直播排期"), szBkImagePath, 55, 14);

	int nItemNum = qMin(7, obList.size());

	// 其它分类
	for (int i = 0; i < nItemNum/*obList.size()*/; i++)
	{
		m_pCustomButtonGroup->AddTabButton(obList[i].qsCategoryName, szBkImagePath, 28, 14);
	}


	m_pCustomButtonGroup->AddHorizontalSpacer();
	ui.verticalLayout->insertWidget(1, m_pCustomButtonGroup);

	QHBoxLayout *pHBoxLayout = new QHBoxLayout(NULL);
	pHBoxLayout->setSpacing(0);
	pHBoxLayout->setContentsMargins(0, 0, 0, 0);

	QFrame *line = new QFrame(this);
	line->setObjectName(QStringLiteral("line"));
	line->setMinimumSize(QSize(610, 1));
	line->setMaximumSize(QSize(610, 1));
	line->setStyleSheet(QStringLiteral("border-color: rgba(47, 47, 47, 1);"));
	line->setFrameShadow(QFrame::Plain);
	line->setFrameShape(QFrame::HLine);

	pHBoxLayout->addWidget(line);
	ui.verticalLayout->insertLayout(2, pHBoxLayout);

	connect(m_pCustomButtonGroup, SIGNAL(signalTabButtonSwitch(int)), SLOT(slotTabButtonSwitch(int)));
	CLiveScheduleWidget *pLiveScheduleWidget = new CLiveScheduleWidget(&m_swModuleContainer);
	connect(pLiveScheduleWidget, SIGNAL(sigOnGroupNumberChanged()), this, SLOT(slotOnGroupNumberChanged()));
	pLiveScheduleWidget->SetModuleID(0);
	connect(this, SIGNAL(signalModuleSwitch(int)), pLiveScheduleWidget, SLOT(slotModuleSwitch(int)));
	connect(pLiveScheduleWidget, &CLiveScheduleWidget::signalItemSelection, this, &CLiveContentSelectionDlg::slotItemSelection);
	m_swModuleContainer.addWidget(pLiveScheduleWidget);

	for (int i = 0; i < nItemNum/*obList.size()*/; i++)
	{
		CUniversalSW *pUniversalSW = new CUniversalSW(&m_swModuleContainer);
		pUniversalSW->SetModuleID(i + 1);
		pUniversalSW->SetCategoryID(obList[i].nCategoryID);
		pUniversalSW->SetCategoryName(obList[i].qsCategoryName);
		connect(this, SIGNAL(signalModuleSwitch(int)), pUniversalSW, SLOT(slotModuleSwitch(int)));
		connect(pUniversalSW, &CUniversalSW::signalItemSelection, this, &CLiveContentSelectionDlg::slotItemSelection);
		m_swModuleContainer.addWidget(pUniversalSW);
	}

	ui.verticalLayout->insertWidget(3, &m_swModuleContainer);

	m_pCustomButtonGroup->SetDefaultDownBtn(0);
}

void CLiveContentSelectionDlg::slotOnGroupNumberChanged()
{
	ui.label_timeDuration->setText(QStringLiteral("(%1小时内开赛数据)").arg(g_dataCentre.stUserInformation.nLiveScheduleScope));
}

//void CLiveContentSelectionDlg::SwitchTabModule(EN_MODULE_TYPE eModuleType)
//{
//	m_swModuleContainer.setCurrentIndex(eModuleType);
//	emit signalModuleSwitch(eModuleType);
//}
//
//
//void CLiveContentSelectionDlg::slotTabButtonSwitch(int index)
//{
//	SwitchTabModule((EN_MODULE_TYPE)index);
//}

void CLiveContentSelectionDlg::SwitchTabModule(int nModuleID)
{
	m_swModuleContainer.setCurrentIndex(nModuleID);
	emit signalModuleSwitch(nModuleID);
}


void CLiveContentSelectionDlg::slotTabButtonSwitch(int index)
{
	SwitchTabModule(index);
}

void CLiveContentSelectionDlg::slotEditorBtnClicked(int index)
{
	this->accept();

	QString qsCategoryName = QString::fromLocal8Bit("足球-英超");
	QString qsContentName = QString::fromLocal8Bit("曼城vs斯托克城");
	//emit signalLiveContentUpdate(qsCategoryName, qsContentName);
}

void CLiveContentSelectionDlg::slotItemSelection(ST_MATCH_INFO &stMatchInfo)
{
	this->accept();
	emit signalLiveContentUpdate(stMatchInfo);

	qDebug("test signal-slot mechanism\n");
}

void CLiveContentSelectionDlg::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		return;
	}
}
