#pragma once

#include <QDialog>
#include "ui_CTestDlg.h"

class CTestDlg : public QDialog
{
	Q_OBJECT

public:
	CTestDlg(QWidget *parent = Q_NULLPTR);
	~CTestDlg();

private:
	Ui::CTestDlg ui;
};
