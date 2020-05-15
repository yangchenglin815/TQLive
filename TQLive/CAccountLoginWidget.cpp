#include "CAccountLoginWidget.h"

CAccountLoginWidget::CAccountLoginWidget(QWidget *parent)
	: QWidget(parent)
	, m_bIsPeekState(false)
{
	ui.setupUi(this);
	connect(ui.lb_peek_password, &CCustomLabel::clicked, this, &CAccountLoginWidget::slotPeekPassword);

	ui.le_password->setEchoMode(QLineEdit::Password);
	ui.le_password->setMaxLength(25);

	connect(ui.cb_remember_pwd, &QCheckBox::stateChanged, this, &CAccountLoginWidget::slotRemeberPwdStateChanged);
	connect(ui.cb_auto_login, &QCheckBox::stateChanged, this, &CAccountLoginWidget::slotAutoLoginStateChanged);
}

CAccountLoginWidget::~CAccountLoginWidget()
{
}

void CAccountLoginWidget::slotPeekPassword()
{
	m_bIsPeekState = !m_bIsPeekState;

	QString qsFilePath;

	if (m_bIsPeekState)
	{
		qsFilePath = ":/TQLive/images/lb_peek_password.png";
		ui.le_password->setEchoMode(QLineEdit::Normal);

	}
	else
	{
		qsFilePath = ":/TQLive/images/lb_unpeek_password.png";
		ui.le_password->setEchoMode(QLineEdit::Password);
	}

	QPixmap pixmap(qsFilePath);
	ui.lb_peek_password->setPixmap(pixmap);
}

void CAccountLoginWidget::slotRemeberPwdStateChanged(int state)
{
	if (Qt::Unchecked == state)
	{
		ui.cb_auto_login->setChecked(false);
	}
}

void CAccountLoginWidget::slotAutoLoginStateChanged(int state)
{
	if (Qt::Checked == state)
	{
		ui.cb_remember_pwd->setChecked(true);
	}
}
