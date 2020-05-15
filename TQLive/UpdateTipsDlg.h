#pragma once

#include <QDialog>
#include "ui_UpdateTipsDlg.h"
#include "CDataCentre.h"

class UpdateTipsDlg : public QDialog
{
	Q_OBJECT

public:
	UpdateTipsDlg(QWidget *parent);
	~UpdateTipsDlg();

	void InitData(const ST_VER_INFO &verInfo);

private slots:
	void onUpdateTQLive();

private:
	void InitConnect();
	void InitUI();

private:
	Ui::UpdateTipsDlg ui;
	QString m_pUpdateDownloadUrl;
};
