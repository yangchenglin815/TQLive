#pragma once

#include <QPushButton>
#include "ui_CustomUnderLineButton.h"

class CustomUnderLineButton : public QPushButton
{
	Q_OBJECT

public:
	CustomUnderLineButton(QWidget *parent = Q_NULLPTR);
	~CustomUnderLineButton();

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	void InitUI();
	void InitConnect();

private:
	Ui::CustomUnderLineButton ui;
};
