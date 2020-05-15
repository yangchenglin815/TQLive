#pragma once

#include <QDialog>
#include "ui_ExitAppTipsDlg.h"

class ExitAppTipsDlg : public QDialog
{
	Q_OBJECT

public:
	ExitAppTipsDlg(QWidget *parent);
	~ExitAppTipsDlg();

	void InitPage();

signals:
	void onSigConfirmClicked();

private slots:
    void onConfirmClicked();

private:
	void InitUI();
	void InitConnect();

private:
	Ui::ExitAppTipsDlg ui;
};
