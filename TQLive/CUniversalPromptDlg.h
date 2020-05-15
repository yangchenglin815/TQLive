#pragma once

#include <QDialog>
#include "ui_CUniversalPromptDlg.h"

class CUniversalPromptDlg : public QDialog
{
	Q_OBJECT

public:
	static CUniversalPromptDlg * DisplayPromptDlg(QWidget *parent, const QString &qsPromptText, int nUserData = -1, const QString &qsTitle = QString::fromLocal8Bit("提示"));
	CUniversalPromptDlg(QWidget *parent = Q_NULLPTR);
	~CUniversalPromptDlg();
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
	Ui::CUniversalPromptDlg ui;
	static CUniversalPromptDlg *m_pPromptDlg;
	int m_nUserData;
};
