/***********************************************************************************
*								RedEnvelopeDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 红包功能对话框
***********************************************************************************/

#include "RedEnvelopeDlg.h"
#include "FramelessHelper.h"
#include "EnumData.h"

#include <QIntValidator>
#include <QMessageBox>

RedEnvelopeDlg::RedEnvelopeDlg(QWidget *parent)
	: QDialog(parent)
	, m_curr_balance(0)
	, m_pTQPayConfirmDlg(NULL)
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

RedEnvelopeDlg::~RedEnvelopeDlg()
{

}

void RedEnvelopeDlg::onRechargeClicked()
{

}

void RedEnvelopeDlg::onConfirmClicked()
{
	if (ui.lineEdit_amount->text().isEmpty())
	{
		ui.lineEdit_amount->setFocus();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("红包总金额不能为空！"));
		return;
	}
	else if (ui.lineEdit_amount->text().toInt() == 0)
	{
		ui.lineEdit_amount->setFocus();
		ui.lineEdit_amount->clear();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("红包总金额不能为0！"));
		return;
	}
	else if (ui.lineEdit_amount->text().toInt() > m_curr_balance)
	{
		ui.lineEdit_amount->setFocus();
		ui.lineEdit_amount->clear();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("当前余额不足！"));
		return;
	}
	else if (ui.lineEdit_number->text().isEmpty())
	{
		ui.lineEdit_number->setFocus();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("红包个数不能为空！"));
		return;
	}
	else if (ui.lineEdit_number->text().toInt() == 0)
	{
		ui.lineEdit_number->setFocus();
		ui.lineEdit_number->clear();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("红包个数不能为0！"));
		return;
	}
	else if (ui.lineEdit_number->text().toInt() > ui.lineEdit_amount->text().toInt())
	{
		ui.lineEdit_number->setFocus();
		ui.lineEdit_number->clear();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("红包个数不能超过红包总金额！"));
		return;
	}
	else if (ui.lineEdit_title->text().isEmpty())
	{
		ui.lineEdit_title->setFocus();
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("标题不能为空！"));
		return;
	}

	if (m_pTQPayConfirmDlg == NULL)
	{
		m_pTQPayConfirmDlg = new TQPayConfirmDlg(DIALOG_PAY_CONFIRM_SIMPLE_OPERATOR, this);
	}
	m_pTQPayConfirmDlg->exec();

	this->accept();
}

void RedEnvelopeDlg::InitUI()
{
	QIntValidator *intValidator = new QIntValidator;
	intValidator->setRange(1, 2000);
	ui.lineEdit_amount->setValidator(intValidator);
	ui.lineEdit_amount->setMaxLength(4);

	ui.lineEdit_number->setValidator(intValidator);
	ui.lineEdit_number->setMaxLength(4);

	ui.lineEdit_title->setMaxLength(10);

	ui.lbl_current_goldVal->setText(QStringLiteral("当前余额：%1探球币").arg(m_curr_balance));
}

void RedEnvelopeDlg::InitConnect()
{
	connect(ui.btn_close, &QPushButton::clicked, this, &QDialog::close);
	connect(ui.btn_cancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
	connect(ui.btn_recharge, SIGNAL(clicked()), this, SLOT(onRechargeClicked()));
}
