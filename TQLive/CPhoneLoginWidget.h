#pragma once

#include <QWidget>
#include "ui_CPhoneLoginWidget.h"

class CPhoneLoginWidget : public QWidget
{
	Q_OBJECT

public:
	CPhoneLoginWidget(QWidget *parent = Q_NULLPTR);
	~CPhoneLoginWidget();

public slots:
	void slotPeekPassword();
	void slotRemeberPwdStateChanged(int state);
	void slotAutoLoginStateChanged(int state);

private:
	bool m_bIsPeekState;

public:
	Ui::CPhoneLoginWidget ui;
};
