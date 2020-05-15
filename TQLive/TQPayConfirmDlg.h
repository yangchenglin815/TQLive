#pragma once

#include <QDialog>
#include "ui_TQPayConfirmDlg.h"

class TQPayConfirmDlg : public QDialog
{
	Q_OBJECT

public:
	explicit TQPayConfirmDlg(int operatorType, QWidget *parent);
	~TQPayConfirmDlg();

private slots:
    void onPayConfirmClicked();
	void onRechargeClicked();
  
private:
	void InitUI();
	void InitConnect();

private:
	Ui::TQPayConfirmDlg ui;
	int m_nOperatorType;
};
