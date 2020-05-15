/***********************************************************************************
*								BroadcastReminderDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 开播提醒对话框
***********************************************************************************/
#include "BroadcastReminderDlg.h"
#include "FramelessHelper.h"
#include "HttpAgent.h"
#include "GlobalSignal.h"

BroadcastReminderDlg::BroadcastReminderDlg(QWidget *parent)
	: QDialog(parent)
	, m_nRemainingTimes(0)
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

BroadcastReminderDlg::~BroadcastReminderDlg()
{

}

void BroadcastReminderDlg::InitData()
{
	HttpAgent::instance()->RequestOnGetReminderContent();
}

void BroadcastReminderDlg::onBroadcastReminder(int nStatusCode, const QString &qsMsg)
{
	ui.button_send->setEnabled(true);

	if (nStatusCode == 200)
	{
		this->close();
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatusCode, qsMsg);
	}
}

void BroadcastReminderDlg::onGetReminderContent(int nStatusCode, const QString & qsMsg, int nRemainedTime, const void * userData)
{
	Q_UNUSED(userData)

	if (nStatusCode == 200)
	{

		m_nRemainingTimes = nRemainedTime;
		ui.textEdit_content->setText(qsMsg);
		if (nRemainedTime == 0) {
			this->exec();
		}
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatusCode, qsMsg);
	}
}

void BroadcastReminderDlg::InitConnect()
{
	connect(HttpAgent::instance(), SIGNAL(onSigBroadcastReminder(int, const QString &)), this, SLOT(onBroadcastReminder(int, const QString &)));
	connect(HttpAgent::instance(), SIGNAL(onSigGetReminderContent(int, const QString &, int, const void *)), 
		this, SLOT(onGetReminderContent(int, const QString &, int, const void *)));

	connect(ui.button_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.button_send, &QPushButton::clicked, this, [&]() {
		ui.button_send->setEnabled(false);
		HttpAgent::instance()->RequestOnBroadcastReminder();
	});
}

void BroadcastReminderDlg::InitUI()
{
	ui.button_close->setCursor(Qt::PointingHandCursor);
	ui.button_send->setCursor(Qt::PointingHandCursor);
}
