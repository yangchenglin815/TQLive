#pragma once

#include <QDialog>
#include "ui_TQLiveAboutDlg.h"

class TQLiveAboutDlg : public QDialog
{
	Q_OBJECT

public:
	TQLiveAboutDlg(QWidget *parent);
	~TQLiveAboutDlg();

	void InitData(bool updateFinished);

private slots:
    void onCheckForUpdates();

private:
	void InitUI();
	void InitConnect();

private:
	Ui::TQLiveAboutDlg ui;
};
