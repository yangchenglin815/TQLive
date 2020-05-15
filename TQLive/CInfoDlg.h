#pragma once

#include <QDialog>
#include "ui_CInfoDlg.h"

class CInfoDlg : public QDialog
{
	Q_OBJECT

public:
	CInfoDlg(QWidget *parent = Q_NULLPTR);
	~CInfoDlg();

private:
	Ui::CInfoDlg ui;
};
