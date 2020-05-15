#pragma once

#include <QDialog>
#include "ui_CLiveContentSelectPromptDlg.h"

class CLiveContentSelectPromptDlg : public QDialog
{
	Q_OBJECT

public:
	static CLiveContentSelectPromptDlg * DisplayPromptDlg(QWidget *parent, const QString &qsPromptText, int nUserData = -1, const QString &qsTitle = QString::fromLocal8Bit("提示"));
	CLiveContentSelectPromptDlg(QWidget *parent = Q_NULLPTR);
	~CLiveContentSelectPromptDlg();
	void SetPromptDlgDisplayContent(const QString &qsPromptText, int nUserData = -1, const QString &qsTitle = QString::fromLocal8Bit("提示"));

public slots:
	void slotTurnoffDlg();
	void slotAcceptSelection();
	void slotRejectSelection();
	void slotDefaultSelection();

signals:
	void signalTurnoffDlg(int nUserData);
	

protected:
	void keyPressEvent(QKeyEvent *event);

public:
	Ui::CLiveContentSelectPromptDlg ui;
	static CLiveContentSelectPromptDlg *m_pPromptDlg;
	int m_nUserData;
};
