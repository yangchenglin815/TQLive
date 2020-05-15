#pragma once

#include <QDialog>
#include "ui_OffcastStatisticsDlg.h"
#include "CDataCentre.h"

class OffcastStatisticsDlg : public QDialog
{
	Q_OBJECT

public:
	OffcastStatisticsDlg(QWidget *parent);
	~OffcastStatisticsDlg();

	void InitData();

private slots:
    void onOffcastStatistics(int nStatusCode, const QString &qsMsg, const ST_LIVE_STATISTICS &stLiveStatistics, const void *userData);
	void onUpdateLiveEndTime(QString durationTime);

private:
	void InitConnect();
	void InitUI();

private:
	Ui::OffcastStatisticsDlg ui;
};
