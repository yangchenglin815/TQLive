#include "CPhoneLoginWidget.h"
#include <QListView>

CPhoneLoginWidget::CPhoneLoginWidget(QWidget *parent)
	: QWidget(parent)
	, m_bIsPeekState(false)
{
	ui.setupUi(this);
	connect(ui.lb_peek_password, &CCustomLabel::clicked, this, &CPhoneLoginWidget::slotPeekPassword);

	ui.le_password->setEchoMode(QLineEdit::Password);
	ui.le_password->setMaxLength(25);

	ui.cb_area_code->addItem("+86");

	connect(ui.cb_remember_pwd, &QCheckBox::stateChanged, this, &CPhoneLoginWidget::slotRemeberPwdStateChanged);
	connect(ui.cb_auto_login, &QCheckBox::stateChanged, this, &CPhoneLoginWidget::slotAutoLoginStateChanged);

	
	QRegExp regx("[0-9]+$");
	QValidator* validator = new QRegExpValidator(regx, ui.le_phone_number);
	ui.le_phone_number->setValidator(validator);
	ui.le_phone_number->setMaxLength(11);

	ui.cb_area_code->setView(new QListView());
}

CPhoneLoginWidget::~CPhoneLoginWidget()
{
}

void CPhoneLoginWidget::slotPeekPassword()
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

void CPhoneLoginWidget::slotRemeberPwdStateChanged(int state)
{
	if (Qt::Unchecked == state)
	{
		ui.cb_auto_login->setChecked(false);
	}
}

void CPhoneLoginWidget::slotAutoLoginStateChanged(int state)
{
	if (Qt::Checked == state)
	{
		ui.cb_remember_pwd->setChecked(true);
	}
}




