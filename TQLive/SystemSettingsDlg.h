#pragma once

#include <QDialog>
#include "ui_SystemSettingsDlg.h"
#include "TQLiveAboutDlg.h"

class SystemSettingsDlg : public QDialog
{
	Q_OBJECT

public:
	SystemSettingsDlg(QWidget *parent = Q_NULLPTR);
	~SystemSettingsDlg();

private:
	void InitConnect();

signals:
	void onSigShowAboutDlg();
	void onSigShowSettingsIniDlg();
	void onSigShowLogoutDlg();
	void onSigShowFeedbackDlg();

private:
	Ui::SystemSettingsDlg ui;
	TQLiveAboutDlg *m_pTQLiveAboutDlg;
};
