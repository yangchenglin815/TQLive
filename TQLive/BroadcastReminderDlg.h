#pragma once

#include <QDialog>
#include "ui_BroadcastReminderDlg.h"

class BroadcastReminderDlg : public QDialog
{
	Q_OBJECT

public:
	BroadcastReminderDlg(QWidget *parent);
	~BroadcastReminderDlg();

	void InitData();

private slots:
    void onBroadcastReminder(int nStatusCode, const QString &qsMsg);
	void onGetReminderContent(int nStatusCode, const QString &qsMsg, int nRemainedTime, const void *userData);

private:
	void InitConnect();
	void InitUI();

private:
	Ui::BroadcastReminderDlg ui;
	int m_nRemainingTimes;
};
