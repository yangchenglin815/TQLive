#pragma once

#include <QWidget>
#include "ui_CAccountLoginWidget.h"

class CAccountLoginWidget : public QWidget
{
	Q_OBJECT

public:
	CAccountLoginWidget(QWidget *parent = Q_NULLPTR);
	~CAccountLoginWidget();

public slots:
	void slotPeekPassword();
	void slotRemeberPwdStateChanged(int state);
	void slotAutoLoginStateChanged(int state);

private:
	bool m_bIsPeekState;

public:
	Ui::CAccountLoginWidget ui;
};
