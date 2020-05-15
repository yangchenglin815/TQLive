#pragma once

#include <QDialog>
#include "ui_RedEnvelopeDlg.h"
#include "TQPayConfirmDlg.h"

class RedEnvelopeDlg : public QDialog
{
	Q_OBJECT

public:
	RedEnvelopeDlg(QWidget *parent);
	~RedEnvelopeDlg();

private slots:
    void onRechargeClicked();
	void onConfirmClicked();

private:
	void InitUI();
	void InitConnect();

private:
	Ui::RedEnvelopeDlg ui;
	int m_curr_balance;
	TQPayConfirmDlg *m_pTQPayConfirmDlg;
};
