#pragma once

#include <QDialog>
#include "ui_TipsDlg.h"

class TipsDlg : public QDialog
{
	Q_OBJECT

public:
	explicit TipsDlg(int operatorType, QWidget *parent);
	~TipsDlg();

public:
	void setText(const QString &text);
	void setTitle(const QString &title);

protected:
	void showEvent(QShowEvent *event);

signals:
	void onSigForbiddenConfirm();

private:
	void InitConnect();
	void InitUI();

private:
	Ui::TipsDlg ui;
	int m_pOperatorType;
};
