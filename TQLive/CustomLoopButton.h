#pragma once

#include <QPushButton>
#include "EnumData.h"
#include "ui_CustomLoopButton.h"

class CustomLoopButton : public QPushButton
{
	Q_OBJECT

public:
	CustomLoopButton(QWidget *parent = Q_NULLPTR);
	~CustomLoopButton();

public:
	void setLoopType(int type);

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	Ui::CustomLoopButton ui;
	int m_nLoopType;
};
