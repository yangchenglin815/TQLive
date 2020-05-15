/***********************************************************************************
*								OffcastStatisticsDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 关播统计对话框
***********************************************************************************/
#include "OffcastStatisticsDlg.h"
#include "FramelessHelper.h"
#include "HttpAgent.h"
#include "GlobalSignal.h"
#include "CController.h"
#include "window-basic-main.hpp"

OffcastStatisticsDlg::OffcastStatisticsDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	InitConnect();
	InitUI();
}

OffcastStatisticsDlg::~OffcastStatisticsDlg()
{

}

void OffcastStatisticsDlg::InitData()
{
	OBSBasic *parent = dynamic_cast<OBSBasic *>(this->parent());
	CController::GetInstance().WriteToLogFile("acquire live statistical information start.");
	HttpAgent::instance()->RequestOnOffcastStatistics(parent->m_nLiveRecordID);
}

void OffcastStatisticsDlg::onOffcastStatistics(int nStatusCode, const QString &qsMsg, const ST_LIVE_STATISTICS &stLiveStatistics, const void *userData)
{

	if (nStatusCode == 200)
	{
		CController::GetInstance().WriteToLogFile("acquire live statistical information end. liveDuration:%s", stLiveStatistics.qsLiveDuration.toLocal8Bit().constData());

		ui.label_highestHeat_val->setText(QString::number(stLiveStatistics.nSupremeHotValue));
		ui.label_addAttention_val->setText(QString::number(stLiveStatistics.nNewlyAddedAttentionValue));
		ui.label_numberOfGroup_val->setText(QString::number(stLiveStatistics.nNewlyAddedGroupNumValue));
		ui.label_gift_val->setText(QString("%1/%2").arg(stLiveStatistics.nReveivedGifts).arg(stLiveStatistics.nBeanGifts));
		ui.label_live_time->setText(stLiveStatistics.qsLiveDuration);

		this->exec();
	}
	else
	{
		CController::GetInstance().WriteToLogFile("acquire live statistical information end. errorCode:%d errorDesc:%s", nStatusCode, qsMsg.toLocal8Bit().constData());
		GlobalSignal::instance()->onSigCatchException(nStatusCode, qsMsg);
	}
}

void OffcastStatisticsDlg::onUpdateLiveEndTime(QString durationTime)
{
	return;
	if (durationTime.isEmpty())
	{
		ui.label_live_time->setText("00:00:00");
	}
	else
	{
		ui.label_live_time->setText(durationTime);
	}
 
	this->exec();
}

void OffcastStatisticsDlg::InitConnect()
{
	connect(ui.button_close, &QPushButton::clicked, this, &QDialog::close);

	connect(HttpAgent::instance(), SIGNAL(onSigOffcastStatistics(int, const QString &, const ST_LIVE_STATISTICS &, const void *)), 
		this, SLOT(onOffcastStatistics(int, const QString &, const ST_LIVE_STATISTICS &, const void *)));

	connect(GlobalSignal::instance(), SIGNAL(onSigUpdateLiveEndTime(QString)), this, SLOT(onUpdateLiveEndTime(QString)));
}

void OffcastStatisticsDlg::InitUI()
{
	ui.button_close->setCursor(Qt::PointingHandCursor);
}
