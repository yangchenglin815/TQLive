#pragma once

#include <QWidget>
#include "ui_CDisplayUserInfoWidget.h"

class CDisplayUserInfoWidget : public QWidget
{
	Q_OBJECT

public:
	CDisplayUserInfoWidget(QWidget *parent = Q_NULLPTR);
	~CDisplayUserInfoWidget();

public slots:
	void slotForwardToModificationPage();

private:
	Ui::CDisplayUserInfoWidget ui;
};
