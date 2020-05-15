/***********************************************************************************
*								TipsDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-10
*
* Description: 通用弹出提示对话框
***********************************************************************************/

#include "TipsDlg.h"
#include "EnumData.h"
#include "FramelessHelper.h"

TipsDlg::TipsDlg(int operatorType, QWidget *parent)
	: QDialog(parent)
	, m_pOperatorType(operatorType)
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

TipsDlg::~TipsDlg()
{

}

void TipsDlg::setText(const QString & text)
{
	ui.lbl_content->setText(text);
}

void TipsDlg::setTitle(const QString &title)
{
	ui.lbl_tip->setText(title);
}

void TipsDlg::showEvent(QShowEvent *event)
{
	Q_UNUSED(event)
}

void TipsDlg::InitConnect()
{
	connect(ui.btn_close, &QPushButton::clicked, this, &TipsDlg::close);
	connect(ui.btn_cancel, &QPushButton::clicked, this, &TipsDlg::reject);
	connect(ui.btn_confirm, &QPushButton::clicked, this, &TipsDlg::accept);
}

void TipsDlg::InitUI()
{
	QString sText;
	switch (m_pOperatorType)
	{
	case DIALOG_CANCEL_FORBIDDEN_OPERATOR:
		sText = QStringLiteral("确认解禁操作吗？");
		break;
	case DIALOG_DELETE_ROOMMANAGER_OPERATOR:
		sText = QStringLiteral("确认删除操作吗？");
		break;
	case DIALOG_LOGOUT_ACCOUNT_OPERATOR:
		sText = QStringLiteral("是否确定退出登录？");
		break;
	case DIALOG_IDENTITY_TIPS_OPERATOR:
		sText = QStringLiteral("您还没有主播资格!");
		ui.btn_confirm->setStyleSheet("#btn_confirm{background-image:url(:/TQLive/images/apply.png);background-repeat:no-repeat;border:none;} \
                       #btn_confirm:pressed{margin:1px;padding:1px;}");
		ui.lbl_tip->setText(QStringLiteral("主播身份提示"));
		break;
	case DIALOG_INSTANCE_EXIST_OPERATOR:
		sText = QStringLiteral("探球直播助手正在运行。");
		ui.lbl_tip->setText(QStringLiteral("探球直播助手"));
		ui.btn_cancel->setHidden(true);
		break;
	default:
		break;
	}

	ui.lbl_content->setText(sText);
}
