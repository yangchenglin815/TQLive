/***********************************************************************************
*								TQLiveAboutDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 关于对话框
***********************************************************************************/
#include "TQLiveAboutDlg.h"
#include "FramelessHelper.h"
#include "ConstData.h"
#include "CDataCentre.h"
#include "HttpAgent.h"
#include <QMessageBox>

TQLiveAboutDlg::TQLiveAboutDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	InitUI();
	InitConnect();
}

TQLiveAboutDlg::~TQLiveAboutDlg()
{

}

void TQLiveAboutDlg::InitData(bool updateFinished)
{
	if (updateFinished)
	{
		ui.lbl_tips->setText(QStringLiteral("已是最新版本"));
	}
	else
	{
		ui.lbl_tips->setText("");

		this->exec();
	}
}

void TQLiveAboutDlg::onCheckForUpdates()
{
	HttpAgent::instance()->RequestOnGetAppUpToDateVer();
}

void TQLiveAboutDlg::InitUI()
{
	QString versionNo = "";

	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		versionNo = TEST_VERSION_NO;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		versionNo = VERSION_NO;
	}

	ui.lbl_tips->setText("");
	ui.lbl_version->setText(QStringLiteral("探球直播V%1版本").arg(versionNo));
}

void TQLiveAboutDlg::InitConnect()
{
	connect(ui.btn_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.btn_confirm, &QPushButton::clicked, this, &QDialog::accept);
	connect(ui.btn_checkUpdate, SIGNAL(clicked()), this, SLOT(onCheckForUpdates()));
}
