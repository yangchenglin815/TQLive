#pragma once

#include <QDialog>
#include "ui_CPromptDlg.h"

class CPromptDlg : public QDialog
{
	Q_OBJECT

public:
	static CPromptDlg * DisplayPromptDlg(QWidget *parent, const QString &qsPromptText, int nUserData = -1, const QString &qsTitle = QString::fromLocal8Bit("ÌבÊ¾"));
	CPromptDlg(QWidget *parent = Q_NULLPTR);
	~CPromptDlg();

public slots:
	void slotTurnoffDlg();

signals:
	void signalTurnoffDlg(int nUserData);

protected:
	void keyPressEvent(QKeyEvent *event);

public:
	Ui::CPromptDlg ui;
	static CPromptDlg *m_pPromptDlg;
	int m_nUserData;
};
