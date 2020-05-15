/***********************************************************************************
*								UpdateTipsDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 更新提示对话框
***********************************************************************************/
#include "UpdateTipsDlg.h"
#include "FramelessHelper.h"
#include "CDataCentre.h"
#include "ConstData.h"
#include <QDesktopServices>

UpdateTipsDlg::UpdateTipsDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setModal(true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	InitUI();
	InitConnect();
}

UpdateTipsDlg::~UpdateTipsDlg()
{

}

void UpdateTipsDlg::InitData(const ST_VER_INFO &verInfo)
{
	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		m_pUpdateDownloadUrl = APP_DOWNLOAD_URL_TEST_API;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		m_pUpdateDownloadUrl = APP_DOWNLOAD_URL_API;
	}

	ui.label_updateContent->setText(QStringLiteral("探球直播助手V%1").arg(verInfo.qsVersion));
	ui.textEdit_updateContent->append(verInfo.qsUpdateContent);

	this->exec();
}

void UpdateTipsDlg::onUpdateTQLive()
{
	QDesktopServices::openUrl(QUrl(m_pUpdateDownloadUrl));

	this->accept();
}

void UpdateTipsDlg::InitConnect()
{
	connect(ui.button_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.button_cancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui.button_update, SIGNAL(clicked()), this, SLOT(onUpdateTQLive()));
}

void UpdateTipsDlg::InitUI()
{
	m_pUpdateDownloadUrl = "";
	ui.textEdit_updateContent->clear();
	ui.label_updateContent->setText("");
}
