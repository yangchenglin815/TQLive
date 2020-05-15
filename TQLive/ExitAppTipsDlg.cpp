/***********************************************************************************
*								ExitAppTipsDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 实例退出提示对话框
***********************************************************************************/
#include "ExitAppTipsDlg.h"
#include "FramelessHelper.h"
#include "UserConfig.h"
#include "EnumData.h"

ExitAppTipsDlg::ExitAppTipsDlg(QWidget *parent)
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

ExitAppTipsDlg::~ExitAppTipsDlg()
{

}

void ExitAppTipsDlg::InitPage()
{
	ui.checkBox_noTips->setChecked(false);

	int exitType = UserConfig::instance()->getExitTipsType();
	if (exitType == EXIT_TO_TRAY)
	{
		ui.radioButton_min->setChecked(true);
	}
	else if (exitType == EXIT_APP)
	{
		ui.radioButton_exit->setChecked(true);
	}
}

void ExitAppTipsDlg::onConfirmClicked()
{
	this->close();

	UserConfig::instance()->setExitTipsFlag(!ui.checkBox_noTips->isChecked());

	int exitType = EXIT_TO_TRAY;
	if (ui.radioButton_exit->isChecked())
	{
		exitType = EXIT_APP;
	}
	UserConfig::instance()->setExitTipsType(exitType);

	emit onSigConfirmClicked();
}

void ExitAppTipsDlg::InitUI()
{
	ui.radioButton_min->setChecked(true);
	ui.checkBox_noTips->setChecked(false);
}

void ExitAppTipsDlg::InitConnect()
{
	connect(ui.btn_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.btn_cancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
}
